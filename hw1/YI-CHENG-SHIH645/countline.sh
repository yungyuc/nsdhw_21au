#!/bin/bash

if [ "$#" -lt 1 ]; then
  echo "missing file name !!"
elif [ "$#" -gt 1 ]; then
  echo "only one argument is allowed !!"
else
  file_name="$1"
  len=$(wc -l "${file_name}" | tr -dc '0-9')
  echo "${len}"
fi
