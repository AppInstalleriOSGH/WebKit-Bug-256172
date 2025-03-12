#import <mach-o/loader.h>
#import <mach-o/nlist.h>
#import <mach-o/dyld.h>
#import <mach-o/dyld_images.h>
#import <mach/mach.h>
#import <stdio.h>
#import <dlfcn.h>
#import <stdlib.h>
#import <stdbool.h>
#import <fcntl.h>

void initializeSymbols(void);
void print(char* message);
void printHex(uint64_t value);

int main(void) {
    initializeSymbols();
    
    if ((uint64_t)printf == (uint64_t)dlsym(RTLD_DEFAULT, "printf")) {
        print("GOOD!");
    } else {
        print("BAD!");
    }
    
    write(STDOUT_FILENO, "Hello!!\n", 8);
    printHex(mach_task_self_);
    sleep(60);
    return 0;
}
