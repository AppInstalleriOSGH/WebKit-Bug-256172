#import <mach-o/loader.h>
#import <mach-o/dyld.h>
#import <mach-o/dyld_images.h>
#import <mach-o/nlist.h>
#import <mach/mach.h>
#import <sys/mman.h>
#import <stdio.h>
#import <dlfcn.h>

void crash(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

void* my_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (n--) *d++ = *s++;
    return dest;
}

uint64_t findMainEntryPoint(struct mach_header_64* header) {
    struct load_command* command = (struct load_command*)((uint8_t*)header + 32);
    for(int i = 0; i < header->ncmds > 0; i++) {
        if (command->cmd == LC_MAIN) {
            struct entry_point_command* entryCommand = (struct entry_point_command*)command;
            return entryCommand->entryoff;
        }
        command = (struct load_command *)((void *)command + command->cmdsize);
    }
    return 0;
}

uint32_t generate_b_instruction(uint32_t offset) {
    uint32_t imm26 = offset / 4;
    uint32_t instruction = 0x14000000 | (imm26 & 0x03FFFFFF);
    return instruction;
}

// dyld stuff
typedef void* (*dlsym_func)(void*, char*);

struct arm64_frame {
    struct arm64_frame* previousFrame;
    uint64_t lr;
};

uint64_t findDyldBase(void) {
    struct arm64_frame* frame;
    asm("mov %0, fp" : "=r"(frame));
    while (frame->previousFrame->previousFrame != NULL) frame = frame->previousFrame;
    uint32_t* magicPtr = (uint32_t*)frame->lr;
    while (*magicPtr != 0xfeedface && *magicPtr != 0xfeedfacf) magicPtr -= 1;
    return (uint64_t)magicPtr;
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

// Check if string is "__LINKEDIT"
int is__LINKEDIT(char* s) {
    return s[0] == 95 && s[1] == 95 && s[2] == 76 && s[3] == 73 && s[4] == 78 && s[5] == 75 && s[6] == 69 && s[7] == 68 && s[8] == 73 && s[9] == 84 && s[10] == 0;
}

// Check if string is "/usr/lib/system/libdyld.dylib"
int is__libdyld(char* s) {
    return s[0] == 47 && s[1] == 117 && s[2] == 115 && s[3] == 114 && s[4] == 47 && s[5] == 108 && s[6] == 105 && s[7] == 98 && s[8] == 47 && s[9] == 115 && s[10] == 121 && s[11] == 115 && s[12] == 116 && s[13] == 101 && s[14] == 109 && s[15] == 47 && s[16] == 108 && s[17] == 105 && s[18] == 98 && s[19] == 100 && s[20] == 121 && s[21] == 108 && s[22] == 100 && s[23] == 46 && s[24] == 100 && s[25] == 121 && s[26] == 108 && s[27] == 105 && s[28] == 98 && s[29] == 0;
}

// Check if string is "_dlsym"
int is_dlsym(char* s) {
    return s[0] == 95 && s[1] == 100 && s[2] == 108 && s[3] == 115 && s[4] == 121 && s[5] == 109 && s[6] == 0;
}

struct dyld_all_image_infos* findDyldAllImageInfos(uint64_t dyldBase) {
    struct mach_header_64* header = (struct mach_header_64*)dyldBase;
    struct load_command* command = (struct load_command*)((uint8_t*)header + 32);
    uint64_t slide = 0;
    for(int i = 0; i < header->ncmds > 0; i++) {
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64* segment = (struct segment_command_64*)command;
            struct section_64* section = (struct section_64*)((uint8_t*)segment + sizeof(struct segment_command_64));
            if (is__TEXT(segment->segname) == 1) {
                slide = dyldBase - segment->vmaddr;
            }
            for (int j = 0; j < segment->nsects; j++) {
                if (is__all_image_info__DATA(section->sectname) == 1) {
                    return (struct dyld_all_image_infos*)(section->addr + slide);
                }
                section = (struct section_64*)((uint8_t*)section + sizeof(struct section_64));
            }
        }
        command = (struct load_command *)((void *)command + command->cmdsize);
    }
    return NULL;
}

uint64_t findlibDyldImageAddr(struct dyld_all_image_infos* allImageInfos) {
    for (unsigned int i = 0; i < allImageInfos->infoArrayCount; i++) {
        const struct dyld_image_info* imageInfo = &allImageInfos->infoArray[i];
        if (is__libdyld((char*)imageInfo->imageFilePath) == 1) {
            return (uint64_t)imageInfo->imageLoadAddress;
        }
    }
    return 0;
}

dlsym_func findDlsymAddr(uint64_t baseAddr) {
    struct mach_header_64* header = (struct mach_header_64*)baseAddr;
    struct load_command* command = (struct load_command*)((uint8_t*)header + 32);
    struct segment_command_64* linkedit = NULL;
    struct symtab_command* symtab = NULL;
    uint64_t slide = 0;
    for(int i = 0; i < header->ncmds > 0; i++) {
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64* segment = (struct segment_command_64*)command;
            if (is__TEXT(segment->segname) == 1) {
                slide = baseAddr - segment->vmaddr;
            } else if (is__LINKEDIT(segment->segname) == 1) {
                linkedit = (struct segment_command_64*)command;
            }
        } else if (command->cmd == LC_SYMTAB) {
            symtab = (struct symtab_command*)command;
        }
        command = (struct load_command*)((void*)command + command->cmdsize);
    }
    if (!linkedit || !symtab || slide == 0) return 0;
    char* sym_str_table = (char*)linkedit->vmaddr - linkedit->fileoff + slide + symtab->stroff;
    struct nlist_64* sym_table = (struct nlist_64*)(linkedit->vmaddr - linkedit->fileoff + slide + symtab->symoff);
    for (int i = 0; i < symtab->nsyms; i++) {
        if (sym_table[i].n_value && is_dlsym(&sym_str_table[sym_table[i].n_un.n_strx]) == 1) {
            return (dlsym_func)(sym_table[i].n_value + slide);
        }
    }
    return NULL;
}

void prepareBindings(uint64_t address, dlsym_func dlsym_ptr) {
    struct mach_header_64* header = (void*)address;
    struct load_command* command = (struct load_command*)((uint8_t*)header + 32);
    struct symtab_command* symtab = NULL;
    uint64_t* bindings = NULL;
    for(int i = 0; i < header->ncmds > 0; i++) {
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64* segment = (struct segment_command_64*)command;
            struct section_64* section = (struct section_64*)((uint8_t*)segment + sizeof(struct segment_command_64));
            for (int j = 0; j < segment->nsects; j++) {
                if (section->flags == S_NON_LAZY_SYMBOL_POINTERS) {
                    bindings = (uint64_t*)(address + section->offset);
                }
                section = (struct section_64*)((uint8_t*)section + sizeof(struct section_64));
            }
        } else if (command->cmd == LC_SYMTAB) {
            symtab = (struct symtab_command*)command;
        }
        command = (struct load_command*)((void*)command + command->cmdsize);
    }
    struct nlist_64* sym_table = (struct nlist_64*)(address + symtab->symoff);
    int index = 0;
    for (int i = 0; i < symtab->nsyms; i++) {
        if (sym_table[i].n_type != N_EXT) continue;
        char* name = (char*)(address + symtab->stroff + sym_table[i].n_un.n_strx);
        uint64_t addr = (uint64_t)dlsym_ptr(RTLD_DEFAULT, name + 1);
        if (addr == 0) continue;
        bindings[index] = addr;
        index++;
    }
}

int c_start(uint64_t JITAddress, uint64_t shellcodeAddress, void* machoBytes, size_t execSize) {
    // Find dlsym
    uint64_t dyldBase = findDyldBase();
    struct dyld_all_image_infos* allImageInfos = findDyldAllImageInfos(dyldBase);
    uint64_t libdyldBase = findlibDyldImageAddr(allImageInfos);
    dlsym_func dlsym_ptr = findDlsymAddr(libdyldBase);
    
    // Bind our Mach-O to the external symbols via dlsym
    prepareBindings((uint64_t)machoBytes, dlsym_ptr);
    
    // Align our JIT address, this is where we write our Mach-O
    uint64_t alignedAddress = (JITAddress + 0x4000) & 0xFFFFFFFFFFFFF000;
    
    // Find the main entry point of our Mach-O
    uint64_t entryOff = findMainEntryPoint(machoBytes);
    uint64_t alignOff = alignedAddress - JITAddress;
    
    // Write our Mach-O to our temporary buffer which gets written to the JIT address
    my_memcpy((void*)shellcodeAddress + alignOff, machoBytes, execSize);
    
    // Generate a branch instruction at the top of our shellcode to branch to the main entry point
    uint32_t instruction = generate_b_instruction((uint32_t)(alignedAddress - JITAddress) + (uint32_t)entryOff);
    *(uint32_t*)shellcodeAddress = instruction;
    return 0;
}
