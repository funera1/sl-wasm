#!/bin/bash
WASMEDGE_DIR=$(pwd)/../funera1-WasmEdge/build
WASMEDGE_LIB=$WASMEDGE_DIR/lib/api
WASMEDGE_INCLUDE=$WASMEDGE_DIR/include/api

local_mode=0
restore_mode=0

while (( $# > 0 ))
do
  case $1 in
    -r)
      restore_mode=1
      ;;
    -l)
      local_mode=1
      ;;
    -h)
      echo "-l: local exec"
      echo "-r: restore"
      exit 1
      ;;
    *)
      echo "argument $1"
      ;;
  esac
  shift
done

# sl.wasmがない
if [ ! -f ../sl.wasm ]; then
  echo "Not found sl.wasm"
  exit 1
fi

export LD_LIBRARY_PATH=$WASMEDGE_LIB:$LD_LIBRARY_PATH
clear
if [ $restore_mode -eq 1 ]; then
  ./run-sl-wasm --restore
else
  ./run-sl-wasm
fi

if [ $? -eq 1 ] && [ $local_mode -eq 0 ]; then
  echo "Sending.."
  ./send.sh
fi
