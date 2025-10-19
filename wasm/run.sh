#!/bin/bash
# set -euo pipefail

# Resolve script directory so the script works when invoked from any cwd
BASEDIR="$(cd "$(dirname "$0")" && pwd)"
cd "$BASEDIR"

WASMEDGE_DIR="$BASEDIR/../funera1-WasmEdge/build"
WASMEDGE_LIB="$WASMEDGE_DIR/lib/api"

REMOTE_HOST=${REMOTE_HOST:-openlab-raspi}
REMOTE_PATH=${REMOTE_PATH:-sl-wasm/wasm}
# FILE_GLOB can be set externally (e.g. FILE_GLOB='*.img')
FILE_GLOB=${FILE_GLOB:-'*.img'}
TRANSFER_ON_SUCCESS=${TRANSFER_ON_SUCCESS:-true}
# Control whether to perform the transfer. Can be overridden by -n/--no-transfer
TRANSFER_ENABLED=true

log() { printf "[%s] %s\n" "$(date +%H:%M:%S)" "$*"; }

ensure_prereqs() {
  if [ ! -d "$WASMEDGE_DIR" ]; then
    log "[ERROR] WasmEdge build dir not found: $WASMEDGE_DIR"
    exit 1
  fi

  if [ ! -f "$BASEDIR/../sl.wasm" ]; then
    log "[ERROR] Not found $BASEDIR/../sl.wasm"
    exit 1
  fi

  # Export LD_LIBRARY_PATH safely
  export LD_LIBRARY_PATH="$WASMEDGE_LIB${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH"
}

run_main() {
  # Run the built binary from build/ if present, otherwise fall back to ./main
  set +e
  if [ -x build/main ]; then
    ./build/main "$@" 2> /dev/null
  elif [ -x ./main ]; then
    ./main "$@" 2> /dev/null
  else
    log "Executable not found: build/main or ./main"
    exit 1
  fi
  rc=$?
  set -e
  return $rc
}

transfer_files() {
  # Exit early if no remote host configured
  if [ -z "$REMOTE_HOST" ]; then
    log "REMOTE_HOST not set; skipping transfer. (main exit $1)"
    return
  fi

  if [ "$TRANSFER_ON_SUCCESS" = "true" ] && [ "$1" -ne 0 ]; then
    log "main failed and TRANSFER_ON_SUCCESS=true; skipping transfer."
    return
  fi

  # expand glob
  shopt -s nullglob
  files=( $FILE_GLOB )
  if [ ${#files[@]} -eq 0 ]; then
    log "No files to transfer matching: $FILE_GLOB"
    return
  fi

  # Ensure memory.img is transferred last
  mem="memory.img"
  ordered=()
  for f in "${files[@]}"; do
    if [ "$(basename "$f")" = "$mem" ]; then
      continue
    fi
    ordered+=("$f")
  done
  for f in "${files[@]}"; do
    if [ "$(basename "$f")" = "$mem" ]; then
      ordered+=("$f")
      break
    fi
  done

  if [ -n "$REMOTE_PATH" ]; then
    log "Transferring ${#ordered[@]} file(s) to ${REMOTE_HOST}:${REMOTE_PATH}"
    scp "${ordered[@]}" "${REMOTE_HOST}:${REMOTE_PATH}"
  else
    log "Transferring ${#ordered[@]} file(s) to ${REMOTE_HOST} (remote home)"
    scp "${ordered[@]}" "${REMOTE_HOST}:"
  fi

  scp_rc=$?
  if [ $scp_rc -ne 0 ]; then
    log "scp failed with code $scp_rc"
    exit $scp_rc
  fi
}

main() {
  ensure_prereqs

  log "Starting main"
  run_main "$@"
  rc=$?
  log "main exited with $rc"

  if [ "$TRANSFER_ENABLED" = "true" ]; then
    transfer_files $rc
  else
    log "Transfer disabled by -n/--no-transfer; skipping file transfer (main exit $rc)"
  fi
  exit $rc
}

# Parse script-level flags. Any flags intended for the underlying program
# should come after `--` or be passed after these script options.
# Supported:
#   -n, --no-transfer   : run main but do not transfer files after completion
#   -h, --help          : show usage
parse_script_args() {
  script_args=()
  while [ $# -gt 0 ]; do
    case "$1" in
      -n|--no-transfer)
        TRANSFER_ENABLED=false
        shift
        ;;
      -h|--help)
        cat <<EOF
Usage: $(basename "$0") [--no-transfer] [--] [args...]

Options:
  -n, --no-transfer    Disable file transfer after running the program
  -h, --help           Show this help

Any arguments after -- are passed to the program that run.sh executes.
EOF
        exit 0
        ;;
      --)
        shift
        script_args+=("$@")
        break
        ;;
      -* )
        # Unknown option for script: stop parsing so it can be passed to main
        break
        ;;
      *)
        # First non-option argument belongs to main
        break
        ;;
    esac
  done

  # Remaining positional parameters (not consumed) are the main args
  script_args+=("$@")
  # Replace positional params with main args
  set -- "${script_args[@]:-}"
  # Export back to caller by assigning to MAIN_ARGS array-like via special var
  MAIN_ARGS=("$@")
}

parse_script_args "$@"

main "${MAIN_ARGS[@]:-}"
