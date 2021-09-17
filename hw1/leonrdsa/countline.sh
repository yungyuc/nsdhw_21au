#! /bin/bash

if [ "$#" -lt 1 ]
then
	echo 'missing File name' 
elif [ "$#" -gt 1 ]
then
	echo 'only one argument is allowed' 
else
	#first argument
	fname="$1"
	if [ -f "$fname" ]
	then
		a=$(wc --l < "$fname")
		echo "$a" 
	else
		echo "$fname not found"
	fi
fi