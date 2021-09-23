#!/bin/bash

if [ $# -lt 1 ]
then
    echo "missing file name"
elif [ $# -gt 1 ]
then
    echo "only one argument is allowed"
else
    fname=$1
    if [ -r $fname ]
    then
        lines=$(wc -l $fname)
        echo "$lines lines in $1"
    else
        echo "$1 not found"
    fi
fi

exit 0 
