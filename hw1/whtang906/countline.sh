#!/bin/bash

if [ "$#" -lt 1 ]; then
  echo "missing file name"
elif [ "$#" -gt 1 ]; then
  echo "only one argument is allowed"
else
  if [ -f "$1" ]; then
    wc -l $1 | awk '{printf "%s lines in %s\n", $1, $2}'
  else
    echo "$1 not found"
  fi
fi
