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
#import <objc/runtime.h>
#import <objc/message.h>
#import <sys/mman.h>
#import <arpa/inet.h>

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

id stringWithCString(char* string) {
    Class class = objc_getClass("NSString");
    return class ? ((id(*)(Class, SEL, char*))objc_msgSend)(class, sel_registerName("stringWithCString:"), string) : NULL;
}

id URLWithString(id string) {
    Class class = objc_getClass("NSURL");
    return class ? ((id(*)(Class, SEL, id))objc_msgSend)(class, sel_registerName("URLWithString:"), string) : NULL;
}

id dataWithContentsOfURL(id url) {
    Class class = objc_getClass("NSData");
    return class ? ((id(*)(Class, SEL, id))objc_msgSend)(class, sel_registerName("dataWithContentsOfURL:"), url) : NULL;
}

void* dataBytes(id data) {
    return ((void*(*)(id, SEL))objc_msgSend)(data, sel_registerName("bytes"));
}

size_t dataLength(id data) {
    return ((size_t(*)(id, SEL))objc_msgSend)(data, sel_registerName("length"));
}

id downloadFile(char* urlString) {
    return dataWithContentsOfURL(URLWithString(stringWithCString(urlString)));
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
    
    const char *cString = "Hello, World!";
    
    Class NSStringClass = objc_getClass("NSString");
    SEL initWithUTF8StringSelector = sel_registerName("initWithUTF8String:");
    SEL allocSelector = sel_registerName("alloc");
    
    // Allocate NSString instance
    id nsStringInstance = ((id (*)(Class, SEL))objc_msgSend)(NSStringClass, allocSelector);
    
    // Initialize with C string
    nsStringInstance = ((id (*)(id, SEL, const char *))objc_msgSend)(nsStringInstance, initWithUTF8StringSelector, cString);
    
    print("nsStringInstance");
    printHex((uint64_t)nsStringInstance);
    

    id data = downloadFile("https://apple.com");
    void* bytes = dataBytes(data);
    size_t size = dataLength(data);
    printHex((uint64_t)bytes);
    printHex(size);
//    printf("bytes: %p, size: %zu\n", bytes, size);
    
    
//    typedef void (*test_func)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
//    test_func test = bytes;
//    test(200,200,200,200,200,200,200,200);
    //void crash(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
    
//    ret = vm_protect(mach_task_self_, (vm_address_t)bytes, size, 0, PROT_READ | PROT_EXEC);
//    print(mach_error_string(ret));
    sleep(60);
    return 0;
}
