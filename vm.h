//
//  vm.h
//  OpenVM
//
//  Created by 王文海 on 2020/11/25.
//

#ifndef vm_h
#define vm_h

#include <stdio.h>
// Flags: |0|0|0|0|0|0|0|0|
typedef struct
{

} flag;
// vm
typedef struct
{
    // 4MB RAM
    unsigned short ram[1024 * 1024 * 4];
    // 8MB ROM
    unsigned short rom[1024 * 1024 * 8];
    // 64 registers
    unsigned int r[64];
    // 1024 deepth stack
    unsigned short stack[1024];
    // PC:[1MB] - [5MB]
    unsigned int pc;
    // flag
    flag f;
} vm;

#endif /* vm_h */
