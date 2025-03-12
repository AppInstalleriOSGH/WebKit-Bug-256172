#import <stdio.h>
#import <unistd.h>
#import <dlfcn.h>
#import <mach/mach.h>
#import <objc/runtime.h>
#import <objc/message.h>
#import <stdarg.h>

void initializeSymbols(void);

int main(void) {
    initializeSymbols();
    
    // printf
    printf("mach task self: 0x%x\n", mach_task_self_);
    printf("dlsym: %p\n", dlsym);
    printf("printf: %p\n", printf);
    printf("objc_getClass: %p\n", objc_getClass);
    printf("sel_registerName: %p\n", sel_registerName);
    printf("objc_msgSend: %p\n", objc_msgSend);
    
    // puts
    printf("Hello, world!\n");
    
    sleep(60);
    return 0;
}
