//
//  vm.h
//  OpenVM
//
//  Created by 王文海 on 2020/11/25.
//

#ifndef vm_h
#define vm_h

#include <stdio.h>
#include <stdlib.h>
#include "log.h"
// Flags: |0|0|0|0|0|0|0|0|
typedef struct
{

} flag;
// vm
typedef struct
{
    // Program begining
    unsigned short program_begin;
    // 4MB RAM
    unsigned short ram[1024 * 1024 * 4];
    // Stack pointer
    unsigned short sp;
    // 8MB ROM
    unsigned short rom[1024 * 1024 * 8];
    // 64 registers
    unsigned int r[64];
    // 1024 deepth stack
    unsigned short stack[1024];
    // PC: INT
    unsigned int pc;
    // flag
    flag f;
} vm;
// Initial
void init_vm(vm* vm);
// new vm
vm* new_vm();
// load rom
void load_rom(char* path, vm* vm);
#endif /* vm_h */
