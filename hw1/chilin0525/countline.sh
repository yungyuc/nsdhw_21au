#!/usr/bin/env bash

declare -i arg_nums=$#
declare -i check_num=1

if [ ${arg_nums} -lt ${check_num} ]; then 
    echo "missing file name"
elif [ ${arg_nums} -gt ${check_num} ]; then
    echo "only one argument is allowed"
else
    result=$(wc -l < $1)
    # result=$(awk 'END{print NR}' $1)
    if [ $? -eq 0 ]; then
        echo "$result lines in $1"
    else
        echo "$1 not found"
    fi
fi