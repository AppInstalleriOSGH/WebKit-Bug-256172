#import <mach-o/loader.h>
#import <mach-o/dyld.h>
#import <mach-o/dyld_images.h>
#import <mach/mach.h>
#import <stdio.h>
#import <dlfcn.h>
#import "dyld.h"

// Horrible string comparison functions
// Check if string is "/usr/lib/system/libsystem_platform.dylib"
int is_libsystem_platform(char* s) {
    return s[0] == 47 && s[1] == 117 && s[2] == 115 && s[3] == 114 && s[4] == 47 && s[5] == 108 && s[6] == 105 && s[7] == 98 && s[8] == 47 && s[9] == 115 && s[10] == 121 && s[11] == 115 && s[12] == 116 && s[13] == 101 && s[14] == 109 && s[15] == 47 && s[16] == 108 && s[17] == 105 && s[18] == 98 && s[19] == 115 && s[20] == 121 && s[21] == 115 && s[22] == 116 && s[23] == 101 && s[24] == 109 && s[25] == 95 && s[26] == 112 && s[27] == 108 && s[28] == 97 && s[29] == 116 && s[30] == 102 && s[31] == 111 && s[32] == 114 && s[33] == 109 && s[34] == 46 && s[35] == 100 && s[36] == 121 && s[37] == 108 && s[38] == 105 && s[39] == 98 && s[40] == 0;
}

// Check if string is "/usr/lib/system/libsystem_kernel.dylib"
int is_libsystem_kernel(char* s) {
    return s[0] == 47 && s[1] == 117 && s[2] == 115 && s[3] == 114 && s[4] == 47 && s[5] == 108 && s[6] == 105 && s[7] == 98 && s[8] == 47 && s[9] == 115 && s[10] == 121 && s[11] == 115 && s[12] == 116 && s[13] == 101 && s[14] == 109 && s[15] == 47 && s[16] == 108 && s[17] == 105 && s[18] == 98 && s[19] == 115 && s[20] == 121 && s[21] == 115 && s[22] == 116 && s[23] == 101 && s[24] == 109 && s[25] == 95 && s[26] == 107 && s[27] == 101 && s[28] == 114 && s[29] == 110 && s[30] == 101 && s[31] == 108 && s[32] == 46 && s[33] == 100 && s[34] == 121 && s[35] == 108 && s[36] == 105 && s[37] == 98 && s[38] == 0;
}

// Check if string is "/usr/lib/system/libsystem_malloc.dylib"
int is_libsystem_malloc(char* s) {
    return s[0] == 47 && s[1] == 117 && s[2] == 115 && s[3] == 114 && s[4] == 47 && s[5] == 108 && s[6] == 105 && s[7] == 98 && s[8] == 47 && s[9] == 115 && s[10] == 121 && s[11] == 115 && s[12] == 116 && s[13] == 101 && s[14] == 109 && s[15] == 47 && s[16] == 108 && s[17] == 105 && s[18] == 98 && s[19] == 115 && s[20] == 121 && s[21] == 115 && s[22] == 116 && s[23] == 101 && s[24] == 109 && s[25] == 95 && s[26] == 109 && s[27] == 97 && s[28] == 108 && s[29] == 108 && s[30] == 111 && s[31] == 99 && s[32] == 46 && s[33] == 100 && s[34] == 121 && s[35] == 108 && s[36] == 105 && s[37] == 98 && s[38] == 0;
}

// Check if string is "/usr/lib/system/libsystem_asl.dylib"
int is_libsystem_asl(char* s) {
    return s[0] == 47 && s[1] == 117 && s[2] == 115 && s[3] == 114 && s[4] == 47 && s[5] == 108 && s[6] == 105 && s[7] == 98 && s[8] == 47 && s[9] == 115 && s[10] == 121 && s[11] == 115 && s[12] == 116 && s[13] == 101 && s[14] == 109 && s[15] == 47 && s[16] == 108 && s[17] == 105 && s[18] == 98 && s[19] == 115 && s[20] == 121 && s[21] == 115 && s[22] == 116 && s[23] == 101 && s[24] == 109 && s[25] == 95 && s[26] == 97 && s[27] == 115 && s[28] == 108 && s[29] == 46 && s[30] == 100 && s[31] == 121 && s[32] == 108 && s[33] == 105 && s[34] == 98 && s[35] == 0;
}

// Check if string is "/usr/lib/system/libsystem_c.dylib"
int is_libsystem_c(char* s) {
    return s[0] == 47 && s[1] == 117 && s[2] == 115 && s[3] == 114 && s[4] == 47 && s[5] == 108 && s[6] == 105 && s[7] == 98 && s[8] == 47 && s[9] == 115 && s[10] == 121 && s[11] == 115 && s[12] == 116 && s[13] == 101 && s[14] == 109 && s[15] == 47 && s[16] == 108 && s[17] == 105 && s[18] == 98 && s[19] == 115 && s[20] == 121 && s[21] == 115 && s[22] == 116 && s[23] == 101 && s[24] == 109 && s[25] == 95 && s[26] == 99 && s[27] == 46 && s[28] == 100 && s[29] == 121 && s[30] == 108 && s[31] == 105 && s[32] == 98 && s[33] == 0;
}

// Check if string is "/usr/lib/system/libdyld.dylib"
int is__libdyld(char* s) {
    return s[0] == 47 && s[1] == 117 && s[2] == 115 && s[3] == 114 && s[4] == 47 && s[5] == 108 && s[6] == 105 && s[7] == 98 && s[8] == 47 && s[9] == 115 && s[10] == 121 && s[11] == 115 && s[12] == 116 && s[13] == 101 && s[14] == 109 && s[15] == 47 && s[16] == 108 && s[17] == 105 && s[18] == 98 && s[19] == 100 && s[20] == 121 && s[21] == 108 && s[22] == 100 && s[23] == 46 && s[24] == 100 && s[25] == 121 && s[26] == 108 && s[27] == 105 && s[28] == 98 && s[29] == 0;
}

// Check if string is "__all_image_info__DATA" or "__all_image_info__DATA_DIRTY"
int is__all_image_info__DATA(char* s) {
    return (s[0] == 95 && s[1] == 95 && s[2] == 97 && s[3] == 108 && s[4] == 108 && s[5] == 95 &&
            s[6] == 105 && s[7] == 109 && s[8] == 97 && s[9] == 103 && s[10] == 101 && s[11] == 95 &&
            s[12] == 105 && s[13] == 110 && s[14] == 102 && s[15] == 111 && s[16] == 95 && s[17] == 95 &&
            s[18] == 68 && s[19] == 65 && s[20] == 84 && s[21] == 65 && s[22] == 0) || (s[0] == 95 && s[1] == 95 && s[2] == 97 && s[3] == 108 && s[4] == 108 && s[5] == 95 && s[6] == 105 && s[7] == 109 && s[8] == 97 && s[9] == 103 && s[10] == 101 && s[11] == 95 && s[12] == 105 && s[13] == 110 && s[14] == 102 && s[15] == 111 && s[16] == 95 && s[17] == 95 && s[18] == 68 && s[19] == 65 && s[20] == 84 && s[21] == 65 && s[22] == 95 && s[23] == 68 && s[24] == 73 && s[25] == 82 && s[26] == 84 && s[27] == 89 && s[28] == 0);
}

// Check if string is "__TEXT"
int is__TEXT(char* s) {
    return s[0] == 95 && s[1] == 95 && s[2] == 84 && s[3] == 69 && s[4] == 88 && s[5] == 84 && s[6] == 0;
}

// Check if string is "__DATA"
int is__DATA(char* s) {
    return s[0] == 95 && s[1] == 95 && s[2] == 68 && s[3] == 65 && s[4] == 84 && s[5] == 65 && s[6] == 0;
}

// Returns the dyld base address
// Works by traversing the frames on the main thread until we get the next to last
// which holds the address to the dyld start function
// then traverses the dyld start function until it finds the Mach-O magic
uint64_t getDYLDBase(void) {
    uint64_t fp = (uint64_t)getPrevFP();
    while (true) {
        uint64_t tmp = *(uint64_t*)fp;
        uint64_t tmp2 = *(uint64_t*)tmp;
        if (tmp2 == 0) {
            break;
        }
        fp = tmp;
    }
    uint64_t pc = *(uint64_t*)(fp + 8);
    uint32_t* magicAddr = (uint32_t*)pc;
    while (true) {
        if (*magicAddr == MH_MAGIC || *magicAddr == MH_MAGIC_64) {
            break;
        }
        magicAddr -= 1;
    }
    return (uint64_t)magicAddr;
}

uint64_t getDYLDAllImageInfoAddr(uint64_t dyldBase) {
    struct mach_header_64* header = (struct mach_header_64*)dyldBase;
    struct load_command* command = (struct load_command*)((uint8_t*)header + 32);
    uint64_t slide = 0;
    for(int i = 0; i < header->ncmds > 0; i++) {
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64* segment = (struct segment_command_64*)command;
            struct section_64* section = (struct section_64*)((uint8_t*)segment + sizeof(struct segment_command_64));
            if (is__TEXT(section->segname) == 1) {
                slide = dyldBase - segment->vmaddr;
            }
            for (int j = 0; j < segment->nsects; j++) {
                if (is__all_image_info__DATA(section->sectname) == 1) {
                    return section->addr + slide;
                }
                section = (struct section_64*)((uint8_t*)section + sizeof(struct section_64));
            }
        }
        command = (struct load_command *)((void *)command + command->cmdsize);
    }
    return 0;
}

uint64_t findDYLDImageAddr(struct dyld_all_image_infos* allImageInfos, int (*isImagePath)(char*)) {
    for (unsigned int i = 0; i < allImageInfos->infoArrayCount; i++) {
        const struct dyld_image_info* imageInfo = &allImageInfos->infoArray[i];
        if (isImagePath((char*)imageInfo->imageFilePath) == 1) {
            return (uint64_t)imageInfo->imageLoadAddress;
        }
    }
    return 0;
}

void symbolsInit(uint64_t dyldAllImageInfoAddr) {
    struct dyld_all_image_infos* allImageInfos = (struct dyld_all_image_infos*)dyldAllImageInfoAddr;
    // Find the base addresses of some libraries
    uint64_t libsystem_cBase = findDYLDImageAddr(allImageInfos, is_libsystem_c);
    uint64_t libsystem_mallocBase = findDYLDImageAddr(allImageInfos, is_libsystem_malloc);
    uint64_t libsystem_kernelBase = findDYLDImageAddr(allImageInfos, is_libsystem_kernel);
    uint64_t libsystem_platformBase = findDYLDImageAddr(allImageInfos, is_libsystem_platform);
    uint64_t libdyldBase = findDYLDImageAddr(allImageInfos, is__libdyld);
    
    // TODO: Dynamically get symbol offsets
    uint64_t sleep_addr = libsystem_cBase + 0x186E0;
    uint64_t malloc_addr = libsystem_mallocBase + 0x2280;
    uint64_t dlsym_addr = libdyldBase + 0x12b8;
    uint64_t strcmp_addr = libsystem_platformBase + 0x1f90;
    uint64_t strlen_addr = libsystem_platformBase + 0x1f20;
    uint64_t open_addr = libsystem_kernelBase + 0x2530;
    uint64_t getenv_addr = libsystem_cBase + 0x7b14;
    uint64_t abort_addr = libsystem_cBase + 0x1f630;
    uint64_t write_addr = libsystem_kernelBase + 0x231c;
    uint64_t dup2_addr = libsystem_kernelBase + 0xb3b8;
    uint64_t dlopen_addr = libdyldBase + 0x1234;
    
    // Sets the function pointers
    sleep_ptr = (sleep_func)sleep_addr;
    malloc_ptr = (malloc_func)malloc_addr;
    dlsym_ptr = (dlsym_func)dlsym_addr;
    strcmp_ptr = (strcmp_func)strcmp_addr;
    strlen_ptr = (strlen_func)strlen_addr;
    open_ptr = (open_func)open_addr;
    getenv_ptr = (getenv_func)getenv_addr;
    abort_ptr = (abort_func)abort_addr;
    write_ptr = (write_func)write_addr;
    dup2_ptr = (dup2_func)dup2_addr;
    dlopen_ptr = (dlopen_func)dlopen_addr;
}
