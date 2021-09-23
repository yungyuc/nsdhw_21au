#!/bin/bash

argc=$#

if [ $argc -lt 1 ]; then
	echo "missing file name"
elif [ $argc -gt 1 ]; then
	echo "only one argument is allowed"
else
	file=$1
	if [ -f $file ]; then
		echo "$(cat $file | wc -l) lines in $file"
	else
		echo "$file not found"
	fi
fi

