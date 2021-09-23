#!/bin/bash
startstring="python -c " 
contentstring="
import sys
import os.path
if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))
"
if [ "$PYTHON_BIN" == "python0" ]; then
    exec python0 -c "$contentstring" "$1"
elif [ "$PYTHON_BIN" == "python2" ]; then
    exec python2 -c "$contentstring" "$1"
elif [ "$PYTHON_BIN" == "python3" ]; then
    exec python3 -c "$contentstring" "$1"
fi
