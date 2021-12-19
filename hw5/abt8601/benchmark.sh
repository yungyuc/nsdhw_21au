#!/bin/bash

./benchmark.py | column -t -s $'\t' > performance.txt
