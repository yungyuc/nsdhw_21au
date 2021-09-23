#!/bin/bash

# Check if file exists and count lines
if test -f $1; then 
    echo $1 have $(cat $1 | wc -l) lines
else
    echo No files found
fi
