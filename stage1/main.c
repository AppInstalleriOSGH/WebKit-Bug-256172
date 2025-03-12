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

id stringWithCString(char* string) {
    Class class = objc_getClass("NSString");
    return class ? ((id(*)(Class, SEL, char*))objc_msgSend)(class, sel_registerName("stringWithCString:"), string) : NULL;
}

id defaultManager(void) {
    Class class = objc_getClass("NSFileManager");
    return class ? ((id(*)(Class, SEL))objc_msgSend)(class, sel_registerName("defaultManager")) : NULL;
}

id contentsOfDirectoryAtPath(id fileManager, id path) {
    return ((id(*)(id, SEL, id, id))objc_msgSend)(fileManager, sel_registerName("contentsOfDirectoryAtPath:error:"), path, NULL);
}

id getDescription(id object) {
    return ((id(*)(id, SEL))objc_msgSend)(object, sel_registerName("description"));
}

char* UTF8String(id string) {
    return ((char*(*)(id, SEL))objc_msgSend)(string, sel_registerName("UTF8String"));
}

void printNSObject(id object) {
    print(UTF8String(getDescription(object)));
}

int main(void) {
    initializeSymbols();
    
    write(STDOUT_FILENO, "Hello!!\n", 8);
    printHex("mach task self: ", mach_task_self_);
    printHex("dlsym: ", (uint64_t)dlsym);
    printHex("printf: ", (uint64_t)printf);
    
    printHex("objc_getClass: ", (uint64_t)objc_getClass);
    printHex("sel_registerName: ", (uint64_t)sel_registerName);
    printHex("objc_msgSend: ", (uint64_t)objc_msgSend);
    
    char* homePath = getenv("HOME");
    char* path = combineStrings(homePath, "/Library/Caches/com.apple.WebKit.WebContent");
    
    id pathNSString = stringWithCString(path);
    printNSObject(pathNSString);
    
    id fileManager = defaultManager();
    printNSObject(fileManager);
    
    id contents = contentsOfDirectoryAtPath(fileManager, pathNSString);
    printNSObject(contents);
    
    sleep(60);
    return 0;
}
