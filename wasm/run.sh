#!/bin/bash
WASMEDGE_DIR=/home/funera1/repo/funera1-WasmEdge/build
WASMEDGE_LIB=$WASMEDGE_DIR/lib/api
WASMEDGE_INCLUDE=$WASMEDGE_DIR/include/api

# --- Remote transfer configuration ---
# Set these environment variables to control where the file is transferred.
# Example (fish):
#   set -x REMOTE_USER user; set -x REMOTE_HOST host.example.com; set -x REMOTE_PATH /path/on/remote
# FILE_TO_TRANSFER defaults to "output.log" but you can override it.
# If TRANSFER_ON_SUCCESS is "true" (default), transfer only when ./main exits with code 0.
REMOTE_USER=${REMOTE_USER:-}
REMOTE_HOST=${REMOTE_HOST:-}
REMOTE_PATH=${REMOTE_PATH:-}
FILE_TO_TRANSFER=${FILE_TO_TRANSFER:-output.log}
TRANSFER_ON_SUCCESS=${TRANSFER_ON_SUCCESS:-true}
# -------------------------------------

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
RC=$?

echo "main exited with code ${RC}"

# Decide whether to transfer the file
do_transfer=false
if [ -z "$REMOTE_USER" ] || [ -z "$REMOTE_HOST" ] || [ -z "$REMOTE_PATH" ]; then
  echo "Remote transfer not configured (REMOTE_USER/REMOTE_HOST/REMOTE_PATH). Skipping transfer."
else
  if [ "$TRANSFER_ON_SUCCESS" = "true" ]; then
    if [ "$RC" -eq 0 ]; then
      do_transfer=true
    else
      echo "TRANSFER_ON_SUCCESS=true and main failed -> skipping transfer"
    fi
  else
    do_transfer=true
  fi
fi

if [ "$do_transfer" = true ]; then
  if [ ! -f "$FILE_TO_TRANSFER" ]; then
    echo "File to transfer not found: $FILE_TO_TRANSFER"
    exit $RC
  fi

  echo "Transferring $FILE_TO_TRANSFER to ${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_PATH} ..."
  scp "$FILE_TO_TRANSFER" "${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_PATH}"
  SCP_RC=$?
  if [ "$SCP_RC" -ne 0 ]; then
    echo "scp failed with code $SCP_RC"
    exit $SCP_RC
  else
    echo "Transfer succeeded."
  fi
fi

exit $RC
