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
// Max byte code size, Develop use 4kb, Can configure other value
#ifndef MAX_RAM_SIZE
#define MAX_RAM_SIZE 1024 * 4
#endif
// Max stack deepth
#define MAX_STACK_DEEPTH 1024
// Max register count
#define MAX_REGISTER_COUNT 64
// Internal address
#define INTERNAL_ADDRESS_SIZE 1024
// INSTRUCTIONS_COUNT
#define INSTRUCTIONS_COUNT 0xFF
/**
* Special register address
* @ readme.md section
*/
// Nullptr
#define R_NULL 0
// Flag
#define R_FLAG 1
// Addr register
#define R_ADDR 3
// ACC
#define R_ACC 6
// PC
#define R_PC 10
// SP
#define R_SP 14
// SD
#define R_SD 18
// Exception
#define R_EX 22
/**
* VM
*/
typedef struct __attribute__((__packed__))
{
    // Null Pointer
    byte nptr;
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
    // exception register
    uint32 ex;
    // RAM
    byte ram[MAX_RAM_SIZE];
    // stack register
    uint32 stack[MAX_STACK_DEEPTH];
    // general register
    uint32 r[MAX_REGISTER_COUNT];
    // Flags: |0|0|0|0|0|0|0|0|
    struct
    {
        int fze : 1;
        int feq : 1;
        int fex : 1;
        int f4 : 1;
        int f5 : 1;
        int f6 : 1;
        int f7 : 1;
        int f8 : 1;
    } flag;
} vm;

// new vm
vm *new_vm();
// Initial
void init_vm(vm *vm);
// load byte code
void load_vmbc(char *path, vm *vm);
// run byte code
void run_bc(vm *vm);
// set/get value to ram
void set_ram(uint32 offset, byte value, vm *vm);
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
