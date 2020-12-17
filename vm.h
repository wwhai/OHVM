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
// Develop use 4kb
#define MAX_RAM_SIZE 1024 * 4
// Max stack deepth
#define MAX_STACK_DEEPTH 1024
// Max register count
#define MAX_REGISTER_COUNT 64
// Internal address
#define INTERNAL_ADDRESS_SIZE 1024
// INSTRUCTIONS_COUNT
#define INSTRUCTIONS_COUNT 0xfF
/**
* Special register address
*/
// Null
#define NULLPTR 0
// ACC pointer 4byte 0x01 - 0x05
#define ACC 1 //1
// PC pointer 4byte 0x06 - 0x0A
#define PC 1 + 4 // 5
// Stack Deepth 0x0A - 0x0E
#define SD 1 + 4 + 4 // 9
// Stack pointer
#define SP 1 + 4 + 4 + 4 // 13
// Exception pointer
#define EX 1 + 4 + 4 + 4 + 1 // 15

/**
* VM
*/
typedef struct __attribute__((__packed__))
{
    // RAM
    byte ram[MAX_RAM_SIZE];
    // user programe start address
    uint32 start_address;
    // Bytecode size
    uint32 bc_size;
    // address pointer
    uint32 a;
    // acc
    uint32 acc;
    // pc
    uint32 pc;
    // stack deepth
    uint32 sd;
    // stack pointer
    uint32 sp;
    // exception
    uint32 ex;
    // stack register
    uint32 stack[MAX_STACK_DEEPTH];
    // general register
    uint32 r[MAX_REGISTER_COUNT];
    // Flags: |0|0|0|0|0|0|0|0|
    struct __attribute__((__packed__))
    {
        // Maybe have some flag
        bool f1 : true;
        bool f2 : true;
        bool f3 : true;
        bool f4 : true;
        bool f5 : true;
        bool f6 : true;
        bool f7 : true;
        bool f8 : true;
    } flag;
} vm;
// operate_function
typedef void (*operate_function)(vm *vm);
//
operate_function operate_functions[INSTRUCTIONS_COUNT];
// new vm
vm *new_vm();
// Initial
void init_vm(vm *vm);
// Load
void load_instrucsions(operate_function *operate_functions);
// load byte code
void load_vmbc(char *path, vm *vm);
// run byte code
void run_bc(vm *vm);
// set value to ram
void set_ram(uint32 offset, byte value, vm *vm);
// get value from ram
byte get_ram(uint32 offset, vm *vm);
// reset
void reset_ram(vm *vm);
// set start address
void set_start_address(uint32 value, vm *vm);
uint32 get_start_address(vm *vm);
// set/get an address to/from address register
void set_a(byte value, vm *vm);
uint32 get_a(vm *vm);
// set/get value to/from acc
void set_acc(byte value, vm *vm);
uint32 get_acc(vm *vm);
// set/get value to/from pc
void set_pc(uint32 value, vm *vm);
uint32 get_pc(vm *vm);
// set/get value to/from sd
void set_sd(byte value, vm *vm);
uint32 get_sd(vm *vm);
// set/get value to/from sp
void set_sp(byte value, vm *vm);
uint32 get_sp(vm *vm);
// set/get value to/from ex
void set_ex(byte value, vm *vm);
uint32 get_ex(vm *vm);

#endif
