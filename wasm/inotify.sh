#!/bin/bash

# 監視するディレクトリを指定
WATCH_FILE=$(pwd)/sent-img.zip
echo Watching $WATCH_FILE...

if [ ! -e $WATCH_FILE ]; then
  touch $WATCH_FILE
fi

# inotifywaitでディレクトリを監視
inotifywait -m -e modify,create "$WATCH_FILE" | while read path action file; do
    # zipを解答
    unzip $WATCH_FILE img
    if [ ! -$? -eq 0 ]; then
      echo $WATCH_FILE is not zip file
    fi

    mv img/* .
    rmdir img sent-img.zip

    ./run.sh --restore
done
