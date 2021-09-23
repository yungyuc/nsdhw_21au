#!/bin/bash

if [ "$#" -lt 1 ]; then
	echo "Missing file name."
elif [ "$#" -gt 1 ]; then
	echo "Only one argument is allowed."
else
	fname="$1"
	if [ -f "$fname" ]; then
		lines=$(wc --lines < "$fname")
		echo "$lines"
	else
		echo "$fname not found."
	fi
fi
