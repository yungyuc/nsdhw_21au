#!/usr/bin/env bash

if [ $# -eq 0 ]; 
then
  echo 'missing file name\n'
elif [ $# -gt 1 ]; 
then
  echo 'only one argument is allowed\n'
elif [ ! -f $1 ]; 
then
  echo '$1 not found'
else
  printf "%s lines in %s\n" $(wc -l countline.sh | cut -d ' ' -f 1) $1
fi
