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

char* newLine;

void print(char* message) {
    write(STDOUT_FILENO, message, strlen(message));
    write(STDOUT_FILENO, newLine, 1);
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
    
    // "/Library/Caches/com.apple.WebKit.WebContent/log.txt"
    char* relativePath = malloc_ptr(52);
    static GLOB char relativePath_0[] = {0x2f, 0x4c, 0x69, 0x62, 0x72, 0x61};
    static GLOB char relativePath_1[] = {0x72, 0x79, 0x2f, 0x43, 0x61, 0x63};
    static GLOB char relativePath_2[] = {0x68, 0x65, 0x73, 0x2f, 0x63, 0x6f};
    static GLOB char relativePath_3[] = {0x6d, 0x2e, 0x61, 0x70, 0x70, 0x6c};
    static GLOB char relativePath_4[] = {0x65, 0x2e, 0x57, 0x65, 0x62, 0x4b};
    static GLOB char relativePath_5[] = {0x69, 0x74, 0x2e, 0x57, 0x65, 0x62};
    static GLOB char relativePath_6[] = {0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e};
    static GLOB char relativePath_7[] = {0x74, 0x2f, 0x6c, 0x6f, 0x67, 0x2e};
    static GLOB char relativePath_8[] = {0x74, 0x78, 0x74, 0x0};
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
    for (int i = 0; i < 4; i++) {
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
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        crash(500);
    }
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    newLine = malloc(1);
    newLine[0] = 0xA;
    
    char* test = malloc_ptr(58);
    static GLOB char test_0[] = {0x2f, 0x53, 0x79, 0x73, 0x74, 0x65};
    static GLOB char test_1[] = {0x6d, 0x2f, 0x4c, 0x69, 0x62, 0x72};
    static GLOB char test_2[] = {0x61, 0x72, 0x79, 0x2f, 0x50, 0x72};
    static GLOB char test_3[] = {0x69, 0x76, 0x61, 0x74, 0x65, 0x46};
    static GLOB char test_4[] = {0x72, 0x61, 0x6d, 0x65, 0x77, 0x6f};
    static GLOB char test_5[] = {0x72, 0x6b, 0x73, 0x2f, 0x54, 0x69};
    static GLOB char test_6[] = {0x70, 0x4b, 0x69, 0x74, 0x2e, 0x66};
    static GLOB char test_7[] = {0x72, 0x61, 0x6d, 0x65, 0x77, 0x6f};
    static GLOB char test_8[] = {0x72, 0x6b, 0x2f, 0x54, 0x69, 0x70};
    static GLOB char test_9[] = {0x4b, 0x69, 0x74, 0x0};
    for (int i = 0; i < 6; i++) {
        (test + 0)[i] = test_0[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 6)[i] = test_1[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 12)[i] = test_2[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 18)[i] = test_3[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 24)[i] = test_4[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 30)[i] = test_5[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 36)[i] = test_6[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 42)[i] = test_7[i];
    }
    for (int i = 0; i < 6; i++) {
        (test + 48)[i] = test_8[i];
    }
    for (int i = 0; i < 4; i++) {
        (test + 54)[i] = test_9[i];
    }
    
    // We can now print to the file
    print(helloWorld);
    
    dlopen(test, RTLD_NOW);
    struct dyld_all_image_infos* allImageInfos = (struct dyld_all_image_infos*)dyldAllImageInfoAddr;
    for (unsigned int i = 0; i < allImageInfos->infoArrayCount; i++) {
        const struct dyld_image_info* imageInfo = &allImageInfos->infoArray[i];
        print((char*)imageInfo->imageFilePath);
    }
    sleep(60);
    return 0;
}
