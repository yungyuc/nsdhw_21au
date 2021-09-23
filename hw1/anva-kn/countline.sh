#!/bin/bash
#
# This script replicates the fucntionality of countline.py file
#

num_of_arguments=$#

if [[ $num_of_arguments -lt 1 ]]
then
    echo "missing file name."
elif [[ $num_of_arguments -gt 1 ]]
then
    echo "only one argument is allowed."
else
    filename=$1
    if [[ -f "$filename" ]]
    then
       # echo "$filename exists."
       num_of_lines=$(wc -l < $filename)
       echo "$num_of_lines lines in $filename"
    else    
        echo "$filename not found."
    fi
fi
