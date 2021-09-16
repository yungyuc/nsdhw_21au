#!/bin/bash
if [ $# -eq 1 ]; then
	if [ -f $1 ]; then
        cnt=$(sed -n '$=' $1)
        if [[ -s "$1" && -z "$(tail -c 1 "$1")" ]]; then  # if there are last empty line
		    echo "$cnt lines in $1"
        else
		    echo "$(($cnt-1)) lines in $1"
        fi
	else
		echo "$1 not found"
	fi
elif [ $# -lt 1 ]; then
	echo "missing file name"
elif [ $# -gt 1 ]; then
	echo "only one argument is allowed"
fi