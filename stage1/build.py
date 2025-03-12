#!/usr/bin/env python3

payload = open('stage1', 'rb').read()

js = 'var stage1 = new Uint8Array(['
js += ','.join(map(str, payload))
js += ']);\n'

with open('../stage1.js', 'w') as f:
    f.write(js)
