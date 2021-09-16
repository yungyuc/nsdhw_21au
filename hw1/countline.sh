#!/bin/bash

# if len(sys.argv) < 2:
#     sys.stdout.write('missing file name\n')
# elif len(sys.argv) > 2:
#     sys.stdout.write('only one argument is allowed\n')
# else:
#     fname = sys.argv[1]
#     if os.path.exists(fname):
#         with open(fname) as fobj:
#             lines = fobj.readlines()
#         sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
#     else:
#         sys.stdout.write('{} not found\n'.format(fname))


if [$# -eq 1]; then
    fname = $1
    if [[-f $fname]]; then
        lines = $(wc -1 < $fname)
        echo "$lines lines in $fname"
    else
        echo "$fname not found"
    fi
elif [$# -lt 1]; then
    echo "missing file name"
else
    echo "only one argument is allowed"
fi