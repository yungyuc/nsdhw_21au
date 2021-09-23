#!/bin/bash

# Author: eesss34690
# This is the first question for hw1.
#

# if len(sys.argv) < 2:
#    sys.stdout.write('missing file name\n')
if [ "$#" -lt 1 ]; then
    echo "missing file name"

#elif len(sys.argv) > 2:
#    sys.stdout.write('only one argument is allowed\n')
elif [ "$#" -gt 1 ]; then
    echo "only one argument is allowed"

# else:
#    fname = sys.argv[1]
# if os.path.exists(fname):
#         with open(fname) as fobj:
#             lines = fobj.readlines()
#         sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
else
    fname=$1
    if test -f "$fname"; then
        echo "$(cat $fname | wc -l) lines in $fname"

# else:
#         sys.stdout.write('{} not found\n'.format(fname))
    else
        echo "$fname not found"
    fi
fi
