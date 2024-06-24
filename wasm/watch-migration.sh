#!/bin/bash

# 監視するディレクトリを指定
WATCH_FILE=$(pwd)/sent-img.zip
echo Watching $WATCH_FILE...

pgrep inotify.sh
if [ ! $? -eq 0 ]; then
  kill -9 $(pgrep inotify.sh)
fi

if [ ! -e $WATCH_FILE ]; then
  touch $WATCH_FILE
fi

# inotifywaitでディレクトリを監視
while : ; do
  inotifywait -e modify,create "$WATCH_FILE" | while read path action file; do
      # zipを解答
      unzip $WATCH_FILE img
      if [ ! -$? -eq 0 ]; then
        echo $WATCH_FILE is not zip file
      fi

      ./run.sh -r
  done
done
