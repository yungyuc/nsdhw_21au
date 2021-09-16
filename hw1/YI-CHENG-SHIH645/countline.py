#!/usr/bin/env python3

import sys
import os.path
import subprocess

if os.environ.get("is_subprocess") is None:
    python_executable = os.environ['PYTHON_BIN']
    env = os.environ.copy()
    env.update(
        is_subprocess="1"
    )
    args = [python_executable]
    args += sys.argv
    subprocess.call(args, env=env)
    sys.exit()

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
