#ifndef execute_function_h
#define execute_function_h
#include "vm.h"
// see design.md
void NOP(vm *vm);
void START(vm *vm);
void STOP(vm *vm);
void RSTR(vm *vm);
void TIMER(vm *vm);
void GOTO(vm *vm);
void JNOC(vm *vm);
void CALL(vm *vm);
void BACK(vm *vm);

#endif
