#import <mach-o/loader.h>
#import <mach-o/dyld.h>
#import <mach-o/dyld_images.h>
#import <mach-o/nlist.h>
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

void crash(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

char* combineStrings(char* str1, char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* combined = malloc(len1 + len2 + 1);
    for (int i = 0; i < len1; i++) {
        combined[i] = str1[i];
    }
    for (int i = 0; i < len2; i++) {
        combined[i + len1] = str2[i];
    }
    combined[len1 + len2] = 0;
    return combined;
}

void print(char* message) {
    write(STDOUT_FILENO, message, strlen(message));
    write(STDOUT_FILENO, "\n", 1);
}

void printHex(uint64_t value) {
    char hexStr[17];
    int i;
    for (i = 15; i >= 0; i--) {
        hexStr[i] = "0123456789ABCDEF"[value & 0xF];
        value >>= 4;
    }
    hexStr[16] = '\0';
    print(hexStr);
}

int main(void) {
    sleep(5);

    // Init logger
    char* homePath = getenv("HOME");
    char* path = combineStrings(homePath, "/Library/Caches/com.apple.WebKit.WebContent/log.txt");
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        crash(500,500,500,500,500,500,500,500);
    }
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    
    print("Running via custom Mach-O loader");
    print("WE ARE WEBCONTENT!!");
    print("Hello, World!");
    
    printHex((uint64_t)dup2);
    printHex((uint64_t)write);


    printHex((uint64_t)printf);
    printHex((uint64_t)dup2);
    printHex((uint64_t)write);
    
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
