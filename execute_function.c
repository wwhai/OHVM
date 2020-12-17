#include "execute_function.h"
#include "log.h"
//
void NOP(vm *vm)
{
    log_debug("NOP");
    uint32 pc = get_pc(vm);
    set_pc(pc + 1, vm);
}
void START(vm *vm)
{
    log_debug("START");
    uint32 pc = get_pc(vm);
    set_pc(pc + 1, vm);
}
void STOP(vm *vm)
{
    log_debug("STOP");
    uint32 pc = get_pc(vm);
    set_pc(pc + 1, vm);
    exit(0);
}
void RSTR(vm *vm)
{
    log_debug("RSTR");
    init_vm(vm);
}
void TIMER(vm *vm)
{
    log_debug("TIMER");
}
void GOTO(vm *vm)
{
    log_debug("GOTO");
    // |++++++++| ******** ******** ******** ********|
    set_pc(get_pc(vm) + 5, vm);
}
void JNOC(vm *vm)
{
    log_debug("JNOC");
    // |++++++++| ******** ******** ******** ********|
    set_pc(get_pc(vm) + 5, vm);
}
void CALL(vm *vm)
{
    log_debug("CALL");
    // |++++++++| ******** ******** ******** ********|
    set_pc(get_pc(vm) + 5, vm);
    vm->sp++;
}
void BACK(vm *vm)
{
    log_debug("BACK");
    // |++++++++| ******** ******** ******** ********|
    set_pc(get_pc(vm) + 5, vm);
}
