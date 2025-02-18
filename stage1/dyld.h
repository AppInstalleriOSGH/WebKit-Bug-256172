#import <mach-o/loader.h>
#import <mach-o/dyld.h>
#import <mach-o/dyld_images.h>

#define GLOB __attribute__((section("__TEXT, __text")))

// Doesn't work for me
#define CSTR(x) ({\
        static GLOB char tempstr[] = x;\
        tempstr;\
        })

uint64_t getDYLDBase(void);
uint64_t getDYLDAllImageInfoAddr(uint64_t dyldBase);
uint64_t findDYLDImageAddr(struct dyld_all_image_infos* allImageInfos, int (*isImagePath)(char*));
void symbolsInit(uint64_t dyldAllImageInfoAddr);

// exit with SIGABRT and set x12 to value
// to get value from it's crash log
void crash(uint64_t value);
void* getPrevFP(void);

typedef unsigned int (*sleep_func)(unsigned int);
typedef void* (*malloc_func)(size_t);
typedef void* (*dlsym_func)(void*, char*);
typedef int (*strcmp_func)(char*, char*);
typedef size_t (*strlen_func)(char*);
typedef int (*open_func)(const char*, int, ...);
typedef char* (*getenv_func)(const char*);
typedef void (*abort_func)(void);
typedef size_t (*write_func)(int, const void*, size_t);
typedef int (*dup2_func)(int, int);
typedef void* (*dlopen_func)(const char*, int);

sleep_func sleep_ptr;
malloc_func malloc_ptr;
dlsym_func dlsym_ptr;
strcmp_func strcmp_ptr;
strlen_func strlen_ptr;
open_func open_ptr;
getenv_func getenv_ptr;
abort_func abort_ptr;
write_func write_ptr;
dup2_func dup2_ptr;
dlopen_func dlopen_ptr;

#define sleep(seconds) sleep_ptr(seconds)
#define malloc(size) malloc_ptr(size)
#define dlsym(handle, symbol) dlsym_ptr(handle, symbol)
#define strcmp(str1, str2) strcmp_ptr(str1, str2)
#define strlen(str) strlen_ptr(str)
#define open(filename, flags, ...) open_ptr(filename, flags, ##__VA_ARGS__)
#define getenv(name) getenv_ptr(name)
#define abort() abort_ptr()
#define write(fd, buf, count) write_ptr(fd, buf, count)
#define dup2(oldfd, newfd) dup2_ptr(oldfd, newfd)
#define dlopen(path, mode) dlopen_ptr(path, mode)
