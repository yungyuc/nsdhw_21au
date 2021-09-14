#!/bin/bash

if [ $# -lt 1 ]; then
    echo "missing file name"
elif [ $# -gt 1 ]; then
    echo "only one argument is allowed"
else
    test ! -e $1 && echo "$1 not found" && exit 0
    lines=$(wc -l $1 | awk '{ print $1 }')
    file=$(wc -l $1 | awk '{ print $2 }')
    echo "$lines lines in $file"
fi
