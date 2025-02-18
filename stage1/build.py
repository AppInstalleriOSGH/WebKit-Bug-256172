#!/usr/bin/env python3

import subprocess

payload = open('stage1.bin', 'rb').read()

js = 'var stage1 = new Uint8Array(['
js += ','.join(map(str, payload))
js += ']);\n'

with open('stage1.js', 'w') as f:
    f.write(js)

EXPORTS = [
        {'path': 'stage1.js', 'content_type': 'text/javascript; charset=UTF-8'}
]

subprocess.run(['cp', 'stage1.js', '..'], check=True)
