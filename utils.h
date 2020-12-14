//
//  utils.h
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include "types.h"
#include "vm.h"
#include "cpu.h"
// Operate function
typedef void (*function)(vm *vm);
// Mapping table
typedef struct i_f_mapping
{
    vm_instruction instruction;
    function f;
    struct i_f_mapping *next;
} i_f_mapping;

#endif
