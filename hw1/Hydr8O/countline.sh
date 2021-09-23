#!/bin/bash
filename=$1

if [ ! -f $1 ]; then
    echo "$filename not found"
    exit 1
fi

if [[ $# -eq 0 ]]; then
    echo "missing file name"
    exit 1
fi

wc -l $filename | awk '{print ""$1" lines in '$filename'"}'