#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else
    fname=$1
    if [[ -f $fname ]] ; then
        lines=$( wc -l < $fname )
# We can tell the shell to redirect the $fname file to standard input of the wc -l command.
# This will give us the number of lines without the filename.
        echo "$lines lines in $fname"
    else
        echo "$fname not found"
    fi
fi
