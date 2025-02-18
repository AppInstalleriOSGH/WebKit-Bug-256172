#import <dlfcn.h>
#import <unistd.h>
#import <fcntl.h>
#import "dyld.h"

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

int main(void) {
    uint64_t dyldBase = getDYLDBase();
    uint64_t dyldAllImageInfoAddr = getDYLDAllImageInfoAddr(dyldBase);
    if (dyldAllImageInfoAddr == 0) {
        crash(404);
    }
    symbolsInit(dyldAllImageInfoAddr);
    
    // "HOME"
    char* homeString = malloc(5);
    static GLOB char homeString_0[] = {0x48, 0x4f, 0x4d, 0x45, 0x0};
    for (int i = 0; i < 5; i++) {
        (homeString + 0)[i] = homeString_0[i];
    }
    
    // "/Library/Caches/com.apple.WebKit.WebContent/AAAAAA"
    char* relativePath = malloc(51);
    static GLOB char relativePath_0[] = {0x2f, 0x4c, 0x69, 0x62, 0x72, 0x61};
    static GLOB char relativePath_1[] = {0x72, 0x79, 0x2f, 0x43, 0x61, 0x63};
    static GLOB char relativePath_2[] = {0x68, 0x65, 0x73, 0x2f, 0x63, 0x6f};
    static GLOB char relativePath_3[] = {0x6d, 0x2e, 0x61, 0x70, 0x70, 0x6c};
    static GLOB char relativePath_4[] = {0x65, 0x2e, 0x57, 0x65, 0x62, 0x4b};
    static GLOB char relativePath_5[] = {0x69, 0x74, 0x2e, 0x57, 0x65, 0x62};
    static GLOB char relativePath_6[] = {0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e};
    static GLOB char relativePath_7[] = {0x74, 0x2f, 0x41, 0x41, 0x41, 0x41};
    static GLOB char relativePath_8[] = {0x41, 0x41, 0x0};
    for (int i = 0; i < 6; i++) {
        (relativePath + 0)[i] = relativePath_0[i];
    }
    for (int i = 0; i < 6; i++) {
        (relativePath + 6)[i] = relativePath_1[i];
    }
    for (int i = 0; i < 6; i++) {
        (relativePath + 12)[i] = relativePath_2[i];
    }
    for (int i = 0; i < 6; i++) {
        (relativePath + 18)[i] = relativePath_3[i];
    }
    for (int i = 0; i < 6; i++) {
        (relativePath + 24)[i] = relativePath_4[i];
    }
    for (int i = 0; i < 6; i++) {
        (relativePath + 30)[i] = relativePath_5[i];
    }
    for (int i = 0; i < 6; i++) {
        (relativePath + 36)[i] = relativePath_6[i];
    }
    for (int i = 0; i < 6; i++) {
        (relativePath + 42)[i] = relativePath_7[i];
    }
    for (int i = 0; i < 3; i++) {
        (relativePath + 48)[i] = relativePath_8[i];
    }
    
    // "Hello, World!"
    char* helloWorld = malloc(14);
    static GLOB char helloWorld_0[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c};
    static GLOB char helloWorld_1[] = {0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64};
    static GLOB char helloWorld_2[] = {0x21, 0x0};
    for (int i = 0; i < 6; i++) {
        (helloWorld + 0)[i] = helloWorld_0[i];
    }
    for (int i = 0; i < 6; i++) {
        (helloWorld + 6)[i] = helloWorld_1[i];
    }
    for (int i = 0; i < 2; i++) {
        (helloWorld + 12)[i] = helloWorld_2[i];
    }
    
    // Write a file to home directory + relativePath
    char* homePath = getenv(homeString);
    char* path = combineStrings(homePath, relativePath);
    int fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        crash(500);
    }
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    size_t size = write(fd, helloWorld, strlen(helloWorld));
    sleep(60);
    crash(fd);
    return 0;
}
