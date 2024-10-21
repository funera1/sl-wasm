#!/bin/bash
WASMEDGE_DIR=/home/funera1/repo/funera1-WasmEdge/build
WASMEDGE_LIB=$WASMEDGE_DIR/lib/api
WASMEDGE_INCLUDE=$WASMEDGE_DIR/include/api

if [ ! -d $WASMEDGE_DIR ]; then
    echo "[ERROR] Not found ${WASMEDGE_DIR}"
    exit 1
fi

# sl.wasmがない
if [ ! -f ../sl.wasm ]; then
  echo "Not found sl.wasm"
  exit 1
fi

export LD_LIBRARY_PATH=$WASMEDGE_LIB:$LD_LIBRARY_PATH
# ./main ../sl.wasm
clear
./main $1
