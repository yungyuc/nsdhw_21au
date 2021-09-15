#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "missing file name"
elif [ "$#" -gt 1 ]; then
    echo "only one argument is allowed"
else
    fname=$1
    if [ -f ${fname} ]; then
        wc -l ${fname} | awk '{print $1 " lines in " $2}'
    else
        echo "${fname} not found"
    fi
fi