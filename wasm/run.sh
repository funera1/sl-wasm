#!/bin/bash
WASMEDGE_DIR=/home/funera1/repo/fWasmEdge1/build
WASMEDGE_LIB=$WASMEDGE_DIR/lib/api
WASMEDGE_INCLUDE=$WASMEDGE_DIR/include/api


# export C_INCLUDE_PATH=$WASMEDGE_INCLUDE:$C_INCLUDE_PATH
export CPATH=$WASMEDGE_INCLUDE
gcc main.c \
  -g                  \
  -o main             \
  -I$WASMEDGE_INCLUDE \
  -L$WASMEDGE_LIB     \
  -lwasmedge          \
  -Wl,-rpath, $WASMEDGE_LIB/libwasmedge.so


if [ $? -eq 1 ]; then
  echo "Failed to build"
  exit 1
fi

export LD_LIBRARY_PATH=$WASMEDGE_LIB:$LD_LIBRARY_PATH
./main

