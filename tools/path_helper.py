#!/usr/bin/env python3
import tempfile, subprocess, os, sys

for index in range(2, len(sys.argv)):
    sys.argv[index] = sys.argv[index].replace("/", "\\")

process = subprocess.Popen(sys.argv[1:], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
stdout, stderr = process.communicate()
print(stdout.decode(sys.stdout.encoding))
sys.exit(process.returncode)