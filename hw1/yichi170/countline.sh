#!/bin/bash

args=("$@")

if [ $# -eq 1 ]; then
	if [ -e $1 ]; then
		cl=$(wc -l < $1 | tr -d '[:space:]')
		echo "$cl lines in $1"
	else
		echo "$1 not found"
	fi
elif [ $# -gt 1 ]; then
	echo "only one argument is allowed";
else
    echo "missing file name";
fi
