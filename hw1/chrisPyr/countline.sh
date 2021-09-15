#!/bin/bash

# This script if for nsd_hw1
# To count lines in a file

#check numof arg

if [[ $# < 1 ]];then
    echo "missing file name"
elif [[ $# > 1 ]];then
    echo "only one argument is allowed"
else
    fname=$1
    if [[ -f $fname ]];then
        line=$(wc -l <$fname)
        echo "$line lines in $fname"
    else
        echo "$fname not found"
    fi
fi
