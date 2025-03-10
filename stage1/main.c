#import <mach-o/loader.h>
#import <mach-o/dyld.h>
#import <mach-o/dyld_images.h>
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

// JIT code address, shellcode array address, mach-o address and mach-o size
int c_start(uint64_t JITAddress, uint64_t shellcodeAddress, void* machoBytes, size_t execSize) {
    //crash(execSize,execSize,execSize,execSize,execSize,execSize,execSize,execSize);
    
    uint64_t alignedAddress = (JITAddress + 0x4000) & 0xFFFFFFFFFFFFF000;
    uint64_t entryOff = findMainEntryPoint(machoBytes);
    uint64_t alignOff = alignedAddress - JITAddress;
    
//    uint32_t test = *(uint32_t*)(machoBytes + entryOff);
//    crash(test,test,test,test,test,test,test,test);
    
//    *(uint32_t*)(machoBytes + entryOff) = generate_b_instruction(0);
    
    my_memcpy((void*)shellcodeAddress + alignOff, machoBytes, execSize);
    uint32_t instruction = generate_b_instruction((uint32_t)(alignedAddress - JITAddress) + (uint32_t)entryOff);
    *(uint32_t*)shellcodeAddress = instruction;
    return 0;
}
