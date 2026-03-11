import json
import subprocess
from pathlib import Path
from datetime import datetime

ROOT = Path(__file__).resolve().parents[2]
CONFIG_PATH = ROOT / "automation" / "night_pipeline" / "pipeline_config.json"
LOG_DIR = ROOT / "automation" / "night_pipeline" / "logs"


def run(cmd, cwd=None, check=True, capture=False):
    print(f"\n>>> {cmd}")
    return subprocess.run(
        cmd,
        cwd=cwd,
        shell=True,
        check=check,
        text=True,
        capture_output=capture
    )


def write_status(stage_id: str, status: str, note: str = ""):
    status_file = ROOT / "migration" / "reports" / f"{stage_id}_status.json"
    payload = {
        "stage": stage_id,
        "status": status,
        "timestamp": datetime.now().isoformat(timespec="seconds"),
        "note": note
    }
    status_file.write_text(json.dumps(payload, indent=2, ensure_ascii=False), encoding="utf-8")


def git_has_changes(repo_path: Path) -> bool:
    result = run("git status --porcelain", cwd=repo_path, check=True, capture=True)
    return bool(result.stdout.strip())


def ensure_worktree(base_repo: Path, branch: str, worktree_path: Path, base_branch: str):
    if worktree_path.exists():
        print(f"Worktree exists: {worktree_path}")
        return

    existing = run("git branch --list", cwd=base_repo, check=True, capture=True).stdout
    if branch in existing:
        run(f'git worktree add "{worktree_path}" {branch}', cwd=base_repo)
    else:
        run(f'git worktree add -b {branch} "{worktree_path}" {base_branch}', cwd=base_repo)


def run_stage(base_repo: Path, legacy_root: Path, stage: dict, base_branch: str):
    stage_id = stage["id"]
    worktree_path = Path(stage["worktree_abs"])
    prompt_path = ROOT / stage["prompt_file"]
    prompt_text = prompt_path.read_text(encoding="utf-8")

    prompt_text = prompt_text.replace("__LEGACY_ROOT__", str(legacy_root))
    prompt_text = prompt_text.replace("__TARGET_REPO__", str(base_repo))

    ensure_worktree(base_repo, stage["branch"], worktree_path, base_branch)

    temp_prompt = worktree_path / ".night_stage_prompt.md"
    temp_prompt.write_text(prompt_text, encoding="utf-8")

    log_file = LOG_DIR / f"{stage_id}.log"
    cmd = (
        'codex exec --full-auto --sandbox workspace-write '
        '"Read AGENTS.md and the project memory files first. '
        'Then execute the instructions from .night_stage_prompt.md exactly. '
        'If blocked, write the block clearly and stop only this stage."'
    )

    try:
        result = run(cmd, cwd=worktree_path, check=True, capture=True)
        log_file.write_text(result.stdout + "\\n\\nSTDERR:\\n" + result.stderr, encoding="utf-8")

        if git_has_changes(worktree_path):
            run("git add -A", cwd=worktree_path)
            run(f'git commit -m "{stage["commit_message"]}"', cwd=worktree_path)
            write_status(stage_id, "done", "Changes committed in worktree.")
        else:
            write_status(stage_id, "partial", "No file changes detected.")
    except subprocess.CalledProcessError as e:
        output = (e.stdout or "") + "\\n\\nSTDERR:\\n" + (e.stderr or "")
        log_file.write_text(output, encoding="utf-8")
        write_status(stage_id, "blocked", "Stage failed; see log.")
        return False

    return True


def main():
    cfg = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
    base_repo = Path(cfg["target_repo"])
    legacy_root = Path(cfg["legacy_root"])
    worktree_root = Path(cfg["worktree_root"])
    worktree_root.mkdir(parents=True, exist_ok=True)
    LOG_DIR.mkdir(parents=True, exist_ok=True)

    stages = cfg["stages"]
    for stage in stages:
        stage["worktree_abs"] = str(worktree_root / stage["worktree"])

    continue_on_error = cfg.get("continue_on_error", True)
    base_branch = cfg.get("base_branch", "main")

    for stage in stages:
        print(f"\\n===== RUNNING STAGE {stage['id']} =====")
        ok = run_stage(base_repo, legacy_root, stage, base_branch)
        if not ok and not continue_on_error:
            print("Stopping pipeline due to failure.")
            break

    print("\\nNight pipeline finished.")


if __name__ == "__main__":
    main()