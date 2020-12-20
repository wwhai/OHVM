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
#include "utils.h"
#include "execute_function.h"
// operate function define
typedef void (*operate_function)(vm *vm);
// operate function mapping
operate_function operate_functions[INSTRUCTIONS_COUNT];
//
typedef struct cpu
{
    // some code
} cpu;
//
byte get_instruction(vm *vm);
// return pc next offset bytes
int execute(vm *vm);
//
void load_instrucsions(operate_function *operate_functions);
#endif
