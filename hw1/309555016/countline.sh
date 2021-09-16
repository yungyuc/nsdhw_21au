#!/bin/bash

if [[ $# < 1 ]];then
    echo "missing file name"
elif [[ $# > 1 ]];then
    echo "only one argument is allowed"
else 
    if  test -f $1 ;then
        result=$( cat $1 | wc -l )
        echo "$result lines in $1"
    else
         echo "$1 not found"
    fi
fi