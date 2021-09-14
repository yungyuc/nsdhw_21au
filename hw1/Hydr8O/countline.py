#!/usr/bin/env python3
import sys
import os

env_variable_name = 'PYTHON_BIN'
interpreter_path = os.path.join('/'.join(sys.executable.split('/')[:-1]), os.environ[env_variable_name])
if not (os.path.exists(interpreter_path)):
    raise RuntimeError('exec: ' + os.environ[env_variable_name] + ': not found')

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