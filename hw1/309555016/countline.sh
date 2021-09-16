#!/bin/bash

if [$#<1];then
    echo "missing file name"
elif [$#>1];then
    echo "only one argument is allowed"
else 
    result=${cat $1 | wc -l}
    echo result
fi
