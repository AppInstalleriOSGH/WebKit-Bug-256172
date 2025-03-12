#import <stdio.h>
#import <unistd.h>
#import <dlfcn.h>
#import <mach/mach.h>
#import <objc/runtime.h>
#import <objc/message.h>

void initializeSymbols(void);
void print(char* message);
void printHex(char* message, uint64_t value);
char* combineStrings(char* str1, char* str2);

int main(void) {
    initializeSymbols();
    
    write(STDOUT_FILENO, "Hello!!\n", 8);
    printHex("mach task self: ", mach_task_self_);
    printHex("dlsym: ", (uint64_t)dlsym);
    printHex("printf: ", (uint64_t)printf);
    
    printHex("objc_getClass: ", (uint64_t)objc_getClass);
    printHex("sel_registerName: ", (uint64_t)sel_registerName);
    printHex("objc_msgSend: ", (uint64_t)objc_msgSend);
    
    
    sleep(60);
    return 0;
}
