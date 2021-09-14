#!/bin/bash

fname="$1"

if test -f "$fname"; then
    wc -l < $fname | tr -dc '0-9'
fi
