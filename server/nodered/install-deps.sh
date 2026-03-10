#!/bin/sh
set -eu

if command -v apk >/dev/null 2>&1; then
    apk add --no-cache python3 make g++ sqlite sqlite-dev curl
elif command -v apt-get >/dev/null 2>&1; then
    apt-get update
    apt-get install -y --no-install-recommends python3 make g++ sqlite3 libsqlite3-dev ca-certificates curl
    rm -rf /var/lib/apt/lists/*
else
    echo "Unsupported package manager in base image" >&2
    exit 1
fi

