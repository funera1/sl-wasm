#!/bin/bash
# set -eo pipefail

# Resolve script directory so the script works when invoked from any cwd
BASEDIR="$(cd "$(dirname "$0")" && pwd)"
WASMEDGE_DIR="$BASEDIR/../third_party/funera1-WasmEdge/build"
WASMEDGE_LIB="$WASMEDGE_DIR/lib/api"

if [ ! -d "$WASMEDGE_DIR" ]; then
  echo "[ERROR] Not found $WASMEDGE_DIR"
  exit 1
fi

if [ ! -f ../sl.wasm ]; then
  echo "Not found ../sl.wasm"
  exit 1
fi

# export LD_LIBRARY_PATH="$WASMEDGE_LIB${LD_LIBRARY_PATH:-}"
# export LD_LIBRARY_PATH="$WASMEDGE_LIB${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$WASMEDGE_LIB${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH"

# Config (can be set in environment)
# Use SSH config host alias 'openlab-raspi' by default.
REMOTE_HOST=${REMOTE_HOST:-openlab-raspi}
REMOTE_PATH=${REMOTE_PATH:-sl-wasm/wasm}
FILE_GLOB=${FILE_TO_TRANSFER:-'*.img'}
TRANSFER_ON_SUCCESS=${TRANSFER_ON_SUCCESS:-true}

clear
# Run the built binary from build/ if present, otherwise fall back to ./main
set +e
if [ -x build/main ]; then
  build/main "${1:-}"
elif [ -x ./main ]; then
  ./main "${1:-}"
else
  echo "Executable not found: build/main or ./main"
  exit 1
fi
RC=$?
set -e

if [ -z "$REMOTE_HOST" ]; then
  echo "REMOTE_HOST not set; skipping transfer. (main exit $RC)"
  exit $RC
fi

if [ "$TRANSFER_ON_SUCCESS" = "true" ] && [ "$RC" -ne 0 ]; then
  echo "main failed and TRANSFER_ON_SUCCESS=true; skipping transfer."
  exit $RC
fi

# expand glob
shopt -s nullglob
files=( $FILE_GLOB )
if [ ${#files[@]} -eq 0 ]; then
  echo "No files to transfer matching: $FILE_GLOB"
  exit $RC
fi

if [ -n "$REMOTE_PATH" ]; then
  echo "Transferring ${#files[@]} file(s) to ${REMOTE_HOST}:${REMOTE_PATH}"
  scp "${files[@]}" "${REMOTE_HOST}:${REMOTE_PATH}"
else
  echo "Transferring ${#files[@]} file(s) to ${REMOTE_HOST}: (remote home)"
  scp "${files[@]}" "${REMOTE_HOST}:"
fi

SCP_RC=$?
if [ $SCP_RC -ne 0 ]; then
  echo "scp failed with code $SCP_RC"
  exit $SCP_RC
fi

exit $RC
