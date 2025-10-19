#!/bin/bash

# Resolve script directory so paths work regardless of cwd
BASEDIR="$(cd "$(dirname "$0")" && pwd)"
FILE_TO_WATCH="$BASEDIR/memory.img"

while inotifywait -e modify "$FILE_TO_WATCH"; do
    bash "$BASEDIR/run.sh" -r
done
