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

extern uint64_t libsystem_cBase;
extern uint64_t libsystem_mallocBase;
extern uint64_t libsystem_kernelBase;
extern uint64_t libsystem_platformBase;
extern uint64_t libdyldBase;

extern uint64_t sleep_addr;
extern uint64_t malloc_addr;
extern uint64_t dlsym_addr;
extern uint64_t strcmp_addr;
extern uint64_t strlen_addr;
extern uint64_t open_addr;
extern uint64_t getenv_addr;
extern uint64_t abort_addr;
extern uint64_t write_addr;
extern uint64_t dup2_addr;
extern uint64_t dlopen_addr;

#define sleep(seconds) ((sleep_func)sleep_addr)(seconds)
#define malloc(size) ((malloc_func)malloc_addr)(size)
#define dlsym(handle, symbol) ((dlsym_func)dlsym_addr)(handle, symbol)
#define strcmp(str1, str2) ((strcmp_func)strcmp_addr)(str1, str2)
#define strlen(str) ((strlen_func)strlen_addr)(str)
#define open(filename, flags, ...) ((open_func)open_addr)(filename, flags, ##__VA_ARGS__)
#define getenv(name) ((getenv_func)getenv_addr)(name)
#define abort() ((abort_func)abort_addr)()
#define write(fd, buf, count) ((write_func)write_addr)(fd, buf, count)
#define dup2(oldfd, newfd) ((dup2_func)dup2_addr)(oldfd, newfd)
#define dlopen(path, mode) ((dlopen_func)dlopen_addr)(path, mode)
