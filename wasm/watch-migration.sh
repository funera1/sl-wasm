#!/bin/bash

# 監視するディレクトリを指定
WATCH_FILE=$(pwd)/sent-img.zip
echo Watching $WATCH_FILE...

# pgrep watch-migration.sh
# if [ ! $? -eq 0 ]; then
#   kill -9 $(pgrep inotify.sh)
# fi

if [ ! -e $WATCH_FILE ]; then
  touch $WATCH_FILE
fi

# inotifywaitでディレクトリを監視
while : ; do
  inotifywait -e modify,create "$WATCH_FILE" | while read path action file; do
      # zipを解答
      unzip $WATCH_FILE
      if [ ! -$? -eq 0 ]; then
        echo $WATCH_FILE is not zip file
      fi

      cp img/* .
      rm ./sent-img.zip
      ./run.sh -r -l
  done
done
