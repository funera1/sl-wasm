#!/bin/bash
while : ; do
  kill -SIGINT $(pgrep run-sl-wasm)
  sleep 4
done
