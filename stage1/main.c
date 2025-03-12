#import <stdio.h>
#import <unistd.h>
#import <dlfcn.h>
#import <mach/mach.h>

void initializeSymbols(void);
void print(char* message);
void printHex(char* message, uint64_t value);

int main(void) {
    initializeSymbols();
    
    write(STDOUT_FILENO, "Hello!!\n", 8);
    printHex("mach task self: ", mach_task_self_);
    printHex("dlsym: ", (uint64_t)dlsym);
    printHex("printf: ", (uint64_t)printf);
    sleep(60);
    return 0;
}
