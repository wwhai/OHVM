//
//  cpu.h
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#ifndef cpu_h
#define cpu_h

#include <stdio.h>
#include "types.h"
#include "vm.h"
// Instruction
typedef struct __attribute__((__packed__))
{
    byte operate_code, function_code;
} vm_instruction;
//

typedef struct cpu
{

} cpu;

//
byte get_instruction(vm *vm);
//
vm_instruction parse_instruction(byte instruction);
// return pc next offset bytes
int execute(vm *vm);
#endif
