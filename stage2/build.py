#!/usr/bin/env python3

import subprocess

payload = open('stage2.bin', 'rb').read()

js = 'var stage2 = new Uint8Array(['
js += ','.join(map(str, payload))
js += ']);\n'

with open('stage2.js', 'w') as f:
    f.write(js)

EXPORTS = [
        {'path': 'stage2.js', 'content_type': 'text/javascript; charset=UTF-8'}
]

subprocess.run(['cp', 'stage2.js', '..'], check=True)
