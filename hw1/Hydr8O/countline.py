#!/bin/sh
'''
' 2>/dev/null

interpreter="${PYTHON_BIN}"
exec "$interpreter" "$0" "$@"
'''

import sys
import os

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\n'.format(len(lines[:-1]), fname))
    else:
        sys.stdout.write('{} not found\n'.format(fname))