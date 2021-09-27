#!/bin/bash

$PYTHON_BIN -c "
import sys
import os.path

env = os.environ["PYTHON_BIN"]
if env == "python2" or env == 'python3':
    pass
else:
    print("exec: {}: not found".format(env))
    sys.exit(1)

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
" $@
