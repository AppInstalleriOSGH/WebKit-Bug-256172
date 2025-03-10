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

void crash(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

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

struct dyld_all_image_infos* findDyldAllImageInfos(uint64_t dyldBase) {
    struct mach_header_64* header = (struct mach_header_64*)dyldBase;
    struct load_command* command = (struct load_command*)((uint8_t*)header + 32);
    uint64_t slide = 0;
    for(int i = 0; i < header->ncmds > 0; i++) {
        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64* segment = (struct segment_command_64*)command;
            struct section_64* section = (struct section_64*)((uint8_t*)segment + sizeof(struct segment_command_64));
            if (my_strcmp(segment->segname, "__TEXT") == 0) {
                slide = dyldBase - segment->vmaddr;
            }
            for (int j = 0; j < segment->nsects; j++) {
                if (my_strcmp(section->sectname, "__all_image_info__DATA") == 0 || my_strcmp(section->sectname, "__all_image_info__DATA_DIRTY") == 0) {
                    return (struct dyld_all_image_infos*)(section->addr + slide);
                }
                section = (struct section_64*)((uint8_t*)section + sizeof(struct section_64));
            }
        }
        command = (struct load_command *)((void *)command + command->cmdsize);
    }
    return NULL;
}

uint64_t findDyldImageAddr(struct dyld_all_image_infos* allImageInfos, char* name) {
    for (unsigned int i = 0; i < allImageInfos->infoArrayCount; i++) {
        const struct dyld_image_info* imageInfo = &allImageInfos->infoArray[i];
        if (my_strcmp(imageInfo->imageFilePath, name) == 0) {
            return (uint64_t)imageInfo->imageLoadAddress;
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
            if (my_strcmp(segment->segname, "__TEXT") == 0) {
                slide = baseAddr - segment->vmaddr;
            } else if (my_strcmp(segment->segname, "__LINKEDIT") == 0) {
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
        if (sym_table[i].n_value && my_strcmp(wanted_name, &sym_str_table[sym_table[i].n_un.n_strx]) == 0) {
            return (uint64_t)(sym_table[i].n_value + slide);
        }
    }
    return 0;
}

#define ret (uint64_t)allImageInfos

int main(void) {
    uint64_t dyldBase = findDyldBase();
    struct dyld_all_image_infos* allImageInfos = findDyldAllImageInfos(dyldBase);
    uint64_t libdyldBase = findDyldImageAddr(allImageInfos, "/usr/lib/system/libdyld.dylib");
    uint64_t dlsymAddr = findSymbol(libdyldBase, "_dlsym");
    //uint64_t dlsymAddr = 200;
    crash(ret,dlsymAddr,ret,dlsymAddr,ret,dlsymAddr,ret,dlsymAddr);
    return 22;
}
