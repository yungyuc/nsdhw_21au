#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else
    fname=$1
    if [[ -f $fname ]] ; then
        lines=0;
        while read line; do
            lines=$((lines + 1))
# Arithmetic in bash uses $((...)) syntax.
        done < $fname
        echo "$lines lines in $fname"
    else
        echo "$fname not found"
    fi
fi
