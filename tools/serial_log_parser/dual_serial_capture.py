#!/usr/bin/env python3
"""Simultaneous timestamped serial capture for multiple ports.

This tool is intentionally small and boring:
- open one or more serial ports concurrently
- timestamp lines into per-port and combined logs
- optionally attempt a simple DTR/RTS reset pulse
- survive temporary disconnects while boards reboot/re-enumerate
"""

from __future__ import annotations

import argparse
import json
import sys
import threading
import time
from collections.abc import Iterable
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import TextIO

import serial


def now_iso() -> str:
    return datetime.now().astimezone().isoformat(timespec="milliseconds")


def decode_chunk(chunk: bytes) -> str:
    return chunk.decode("utf-8", errors="replace")


@dataclass(frozen=True)
class PortSpec:
    label: str
    port: str


class LogWriter:
    def __init__(self, output_dir: Path, port_specs: list[PortSpec]) -> None:
        self.output_dir = output_dir
        self.output_dir.mkdir(parents=True, exist_ok=True)
        self.combined_path = self.output_dir / "combined.log"
        self.meta_path = self.output_dir / "capture_meta.json"
        self._lock = threading.Lock()
        self._combined_file: TextIO = self.combined_path.open("w", encoding="utf-8", newline="\n")
        self._port_files: dict[str, TextIO] = {}
        for spec in port_specs:
            safe_label = spec.label.replace(" ", "_")
            self._port_files[safe_label] = (self.output_dir / f"{safe_label}_{spec.port}.log").open(
                "w", encoding="utf-8", newline="\n"
            )

    def write_meta(self, data: dict) -> None:
        self.meta_path.write_text(json.dumps(data, indent=2), encoding="utf-8")

    def write_control(self, message: str) -> None:
        self.write_line("control", "-", message)

    def write_line(self, label: str, port: str, message: str) -> None:
        timestamp = now_iso()
        line = f"{timestamp} [{label} {port}] {message}"
        with self._lock:
            self._combined_file.write(line + "\n")
            self._combined_file.flush()
            if label in self._port_files:
                self._port_files[label].write(f"{timestamp} {message}\n")
                self._port_files[label].flush()
        print(line)

    def close(self) -> None:
        with self._lock:
            for handle in self._port_files.values():
                handle.close()
            self._combined_file.close()


class SerialWorker:
    def __init__(self, spec: PortSpec, baudrate: int, logger: LogWriter) -> None:
        self.spec = spec
        self.baudrate = baudrate
        self.logger = logger
        self._serial: serial.Serial | None = None
        self._buffer = ""
        self._lock = threading.Lock()
        self._last_open_error: str | None = None

    def _build_serial(self) -> serial.Serial:
        ser = serial.Serial()
        ser.port = self.spec.port
        ser.baudrate = self.baudrate
        ser.timeout = 0.1
        ser.write_timeout = 0.1
        ser.dtr = False
        ser.rts = False
        return ser

    def ensure_open(self) -> bool:
        with self._lock:
            if self._serial and self._serial.is_open:
                return True
            try:
                ser = self._build_serial()
                ser.open()
                self._serial = ser
                self._last_open_error = None
                self.logger.write_control(f"{self.spec.label}:{self.spec.port} connected")
                return True
            except (serial.SerialException, OSError) as exc:
                message = str(exc)
                if message != self._last_open_error:
                    self.logger.write_control(f"{self.spec.label}:{self.spec.port} open failed: {message}")
                    self._last_open_error = message
                self._serial = None
                return False

    def close(self) -> None:
        with self._lock:
            self._flush_partial_locked()
            if self._serial is not None:
                try:
                    self._serial.close()
                except (serial.SerialException, OSError):
                    pass
                self._serial = None

    def _flush_partial_locked(self) -> None:
        if self._buffer:
            self.logger.write_line(self.spec.label, self.spec.port, f"<partial> {self._buffer}")
            self._buffer = ""

    def read_once(self) -> None:
        with self._lock:
            ser = self._serial
        if ser is None:
            return

        try:
            chunk = ser.read(256)
            if not chunk:
                return
            text = decode_chunk(chunk)
            self._consume_text(text)
        except (serial.SerialException, OSError) as exc:
            self.logger.write_control(f"{self.spec.label}:{self.spec.port} disconnected: {exc}")
            self.close()

    def _consume_text(self, text: str) -> None:
        text = text.replace("\r\n", "\n").replace("\r", "\n")
        with self._lock:
            self._buffer += text
            parts = self._buffer.split("\n")
            self._buffer = parts.pop()
        for part in parts:
            self.logger.write_line(self.spec.label, self.spec.port, part)

    def pulse_reset(self, mode: str, pulse_ms: int) -> bool:
        with self._lock:
            ser = self._serial
        if ser is None:
            self.logger.write_control(f"{self.spec.label}:{self.spec.port} reset skipped: port not open")
            return False

        try:
            self.logger.write_control(f"{self.spec.label}:{self.spec.port} reset pulse start mode={mode}")
            if mode == "dtr":
                ser.dtr = True
                time.sleep(pulse_ms / 1000.0)
                ser.dtr = False
            elif mode == "dtr-rts":
                ser.rts = True
                ser.dtr = True
                time.sleep(pulse_ms / 1000.0)
                ser.dtr = False
                ser.rts = False
            else:
                raise ValueError(f"unsupported reset mode: {mode}")
            self.logger.write_control(f"{self.spec.label}:{self.spec.port} reset pulse end")
            return True
        except (serial.SerialException, OSError) as exc:
            self.logger.write_control(f"{self.spec.label}:{self.spec.port} reset pulse failed: {exc}")
            self.close()
            return False


def parse_port(value: str) -> PortSpec:
    if "=" not in value:
        raise argparse.ArgumentTypeError("port spec must be LABEL=COMx")
    label, port = value.split("=", 1)
    label = label.strip()
    port = port.strip()
    if not label or not port:
        raise argparse.ArgumentTypeError("port spec must be LABEL=COMx")
    return PortSpec(label=label, port=port)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--port",
        action="append",
        type=parse_port,
        required=True,
        help="Port mapping in the form label=COMx. Repeat for each port.",
    )
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate for all ports.")
    parser.add_argument("--duration", type=float, default=20.0, help="Capture duration in seconds.")
    parser.add_argument(
        "--output-root",
        default="tmp/serial_capture",
        help="Root directory for capture artifacts. A timestamped subdir is created inside it.",
    )
    parser.add_argument(
        "--reset-mode",
        choices=("none", "dtr", "dtr-rts"),
        default="none",
        help="Attempt a simple reset pulse after arming the capture.",
    )
    parser.add_argument(
        "--reset-label",
        action="append",
        default=[],
        help="Label to reset. Repeat to pulse multiple ports. Ignored when --reset-mode=none.",
    )
    parser.add_argument(
        "--reset-gap",
        type=float,
        default=1.0,
        help="Seconds to wait between reset pulses for multiple ports.",
    )
    parser.add_argument(
        "--arm-delay",
        type=float,
        default=1.0,
        help="Seconds to wait after opening ports before the first reset pulse.",
    )
    parser.add_argument(
        "--pulse-ms",
        type=int,
        default=120,
        help="Reset pulse length in milliseconds.",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str]) -> int:
    args = parse_args(argv)
    port_specs: list[PortSpec] = args.port
    if len(port_specs) < 1:
        print("at least one --port is required", file=sys.stderr)
        return 2

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    output_dir = Path(args.output_root) / timestamp
    logger = LogWriter(output_dir, port_specs)
    logger.write_meta(
        {
            "started_at": now_iso(),
            "baud": args.baud,
            "duration_s": args.duration,
            "reset_mode": args.reset_mode,
            "reset_labels": args.reset_label,
            "reset_gap_s": args.reset_gap,
            "arm_delay_s": args.arm_delay,
            "pulse_ms": args.pulse_ms,
            "ports": [{"label": spec.label, "port": spec.port} for spec in port_specs],
        }
    )

    workers = [SerialWorker(spec, args.baud, logger) for spec in port_specs]
    stop_event = threading.Event()

    def reader_loop(worker: SerialWorker) -> None:
        while not stop_event.is_set():
            if worker.ensure_open():
                worker.read_once()
            else:
                time.sleep(0.2)
        worker.close()

    threads = [threading.Thread(target=reader_loop, args=(worker,), daemon=True) for worker in workers]
    for thread in threads:
        thread.start()

    try:
        logger.write_control(f"capture armed output_dir={output_dir}")
        time.sleep(max(args.arm_delay, 0.0))

        if args.reset_mode != "none" and args.reset_label:
            by_label = {worker.spec.label: worker for worker in workers}
            for index, label in enumerate(args.reset_label):
                worker = by_label.get(label)
                if worker is None:
                    logger.write_control(f"reset skipped: unknown label={label}")
                    continue
                worker.pulse_reset(args.reset_mode, args.pulse_ms)
                if index + 1 < len(args.reset_label):
                    time.sleep(max(args.reset_gap, 0.0))

        deadline = time.monotonic() + max(args.duration, 0.0)
        while time.monotonic() < deadline:
            time.sleep(0.1)
    finally:
        stop_event.set()
        for thread in threads:
            thread.join(timeout=1.0)
        logger.write_control("capture finished")
        logger.close()

    print(f"CAPTURE_DIR={output_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
