#!/bin/bash

FILE_TO_WATCH="./memory.img"

while inotifywait -e modify "$FILE_TO_WATCH"; do
    ./run.sh -r
done
