#!/usr/bin/env bash
if [[ -z "$1" ]]; then
    echo "missing file name"
elif [[ ! -z "$2" ]]; then
    echo "only one argument is allowed"
else
    if [[ -f "$1" ]]; then
        ret=$(cat $1 | wc -l | tr -dc '0-9')
        echo "$ret lines in $1"
    else
        echo "$1 not found"
    fi
fi
