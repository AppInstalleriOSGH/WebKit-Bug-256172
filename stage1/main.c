#import <stdio.h>
#import <unistd.h>
#import <dlfcn.h>
#import <mach/mach.h>

void initializeSymbols(void);
void print(char* message);
void printHex(uint64_t value);

int main(void) {
    initializeSymbols();
    
    write(STDOUT_FILENO, "Hello!!\n", 8);
    printHex(mach_task_self_);
    printHex((uint64_t)dlsym);
    sleep(60);
    return 0;
}
