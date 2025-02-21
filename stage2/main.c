#import <mach-o/loader.h>
#import <mach-o/nlist.h>
#import <mach-o/dyld.h>
#import <mach-o/dyld_images.h>
#import <mach/mach.h>
#import <stdio.h>
#import <dlfcn.h>
#import <fcntl.h>

void crash(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7, uint64_t x8, uint64_t x9);

__attribute__((section("__TEXT, __text")))
uint64_t* array = NULL;
__attribute__((section("__TEXT, __text")))
char* newLine = NULL;

#define getString(offset) ((char*)array + 144 + offset)

int same_string(const char *s1, const char *s2) {
    for (int i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') {
            return 1;
        }
    }
    return 0;
}

uint64_t findSymbol(uint64_t baseAddr, char* wanted_name) {
    struct mach_header_64* header = (struct mach_header_64*)baseAddr;
    struct load_command* command = (struct load_command*)((uint8_t*)header + 32);
    struct segment_command_64* linkedit = NULL;
    struct symtab_command* symtab = NULL;
    uint64_t slide = 0;
    for(int i = 0; i < header->ncmds > 0; i++) {
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64* segment = (struct segment_command_64*)command;
            if (same_string(segment->segname, getString(100)) == 1) {
                slide = baseAddr - segment->vmaddr;
            } else if (same_string(segment->segname, getString(107)) == 1) {
                linkedit = (struct segment_command_64*)command;
            }
        } else if (command->cmd == LC_SYMTAB) {
            symtab = (struct symtab_command*)command;
        }
        command = (struct load_command*)((void*)command + command->cmdsize);
    }
    if (!linkedit || !symtab || slide == 0) {
        return 11;
    }
    char* sym_str_table = (char*)linkedit->vmaddr - linkedit->fileoff + slide + symtab->stroff;
    struct nlist_64* sym_table = (struct nlist_64*)(linkedit->vmaddr - linkedit->fileoff + slide + symtab->symoff);
    
    for (int i = 0; i < symtab->nsyms; i++) {
        if (sym_table[i].n_value && same_string(wanted_name, &sym_str_table[sym_table[i].n_un.n_strx]) == 1) {
            return (uint64_t)(sym_table[i].n_value + slide);
        }
    }
    return 22;
}

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

#define sleep(seconds) ((sleep_func)array[7])(seconds)
#define malloc(size) ((malloc_func)array[8])(size)
#define dlsym(handle, symbol) ((dlsym_func)array[9])(handle, symbol)
#define strcmp(str1, str2) ((strcmp_func)array[10])(str1, str2)
#define strlen(str) ((strlen_func)array[11])(str)
#define open(filename, flags, ...) ((open_func)array[12])(filename, flags, ##__VA_ARGS__)
#define getenv(name) ((getenv_func)array[13])(name)
#define abort() ((abort_func)array[14])()
#define write(fd, buf, count) ((write_func)array[15])(fd, buf, count)
#define dup2(oldfd, newfd) ((dup2_func)array[16])(oldfd, newfd)
#define dlopen(path, mode) ((dlopen_func)array[17])(path, mode)

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
    if (newLine) {
        write(STDOUT_FILENO, newLine, 1);
    }
}

int c_start(uint64_t* array_ptr) {
    // Init symbols
    array = array_ptr;
    array[7] = findSymbol(array[2], getString(0));   // _sleep
    array[8] = findSymbol(array[3], getString(7));   // _malloc
    array[9] = findSymbol(array[6], getString(15));  // _dlsym
    array[10] = findSymbol(array[5], getString(22)); // __platform_strcmp
    array[11] = findSymbol(array[5], getString(40)); // __platform_strlen
    array[12] = findSymbol(array[4], getString(58)); // _open
    array[13] = findSymbol(array[2], getString(64)); // _getenv
    array[14] = findSymbol(array[2], getString(72)); // _abort
    array[15] = findSymbol(array[4], getString(79)); // _write
    array[16] = findSymbol(array[4], getString(86)); // _dup2
    array[17] = findSymbol(array[6], getString(92)); // _dlopen
    
    char* homePath = getenv(getString(396));
    char* path = combineStrings(homePath, getString(401));
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        crash(500,500,500,500,500,500,500,500,500,500);
    }
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    newLine = getString(453);
    
    print(path);
    print(getString(455)); // log "Hello, world!" to log.txt
    
    void* libsystem_kernel_handle = dlopen(getString(218), RTLD_NOW); // dlopen libsystem_kernel
    void* sym = dlsym(libsystem_kernel_handle, getString(59)); // getString(59) = "open"
    uint64_t sym_addr = (uint64_t)sym;
    
    sleep(10);
    crash(0,0,(uint64_t)libsystem_kernel_handle,sym_addr,sym_addr,array[15],array[16],array[17],array[13],array[14]);
    return 0;
}
