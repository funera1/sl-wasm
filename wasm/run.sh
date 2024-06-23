#!/bin/bash
WASMEDGE_DIR=$(pwd)/../funera1-WasmEdge/build
WASMEDGE_LIB=$WASMEDGE_DIR/lib/api
WASMEDGE_INCLUDE=$WASMEDGE_DIR/include/api


# export C_INCLUDE_PATH=$WASMEDGE_INCLUDE:$C_INCLUDE_PATH
export CPATH=$WASMEDGE_INCLUDE
gcc main.c \
  -g                  \
  -o run-sl-wasm      \
  -I$WASMEDGE_INCLUDE \
  -L$WASMEDGE_LIB     \
  -lwasmedge          \
  -Wl,-rpath, $WASMEDGE_LIB/libwasmedge.so


# buildが失敗
if [ $? -eq 1 ]; then
  echo "Failed to build"
  exit 1
fi

# sl.wasmがない
if [ ! -f ../sl.wasm ]; then
  echo "Not found sl.wasm"
  exit 1
fi

export LD_LIBRARY_PATH=$WASMEDGE_LIB:$LD_LIBRARY_PATH
./run-sl-wasm
if [ $? -eq 1 ]; then
  ./send.sh
fi
