//
//  vm.h
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#ifndef vm_h
#define vm_h

#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "types.h"
// Max byte code size
// Develop use 1kb
#define MAX_RAM_SIZE 1024
// Max stack deepth
#define MAX_STACK_DEEPTH 1024
// Max register count
#define MAX_REGISTER_COUNT 64

// vm
typedef struct __attribute__((__packed__))
{
    // RAM
    byte ram[MAX_RAM_SIZE];
    // user programe start address
    uint32 start_address;
    // stack pointer
    uint32 sp;
    // address pointer
    uint32 a;
    // acc
    uint32 acc;
    // pc
    uint32 pc;
    // stack deepth
    uint32 sd;
    // exception
    uint32 ex;
    // stack register
    uint32 stack[MAX_STACK_DEEPTH];
    // general register
    uint32 r[MAX_REGISTER_COUNT];
    // Flags: |0|0|0|0|0|0|0|0|
    struct
    {
        // Maybe have some flag
    } flag;
} vm;
// Initial
void init_vm(vm *vm);
// new vm
vm *new_vm();
// load byte code
void load_vmbc(char *path, vm *vm);
// run byte code
void run_bc(vm *vm);
// Instructions table

#endif /* vm_h */
