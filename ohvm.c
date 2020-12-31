
//
//  ohvm.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "ohvm.h"
#include "types.h"
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include "utils.h"
// operate function mapping
operate_function operate_functions[INSTRUCTIONS_COUNT];
// Initial
void init_ohvm(ohvm *vm)
{
  // PC is start at 1024
  // 0-1023 be used internal function
  for (uint32 i = 0; i < MAX_STACK_DEEPTH; i++)
  {
    // log_debug("INIT STACK[%d]", i);
    for (uint32 j = 0; j < MAX_REGISTER_COUNT; j++)
    {
      // log_debug("INIT STACK[%d].R[%d]", i, j);
      vm->stack[i].r[j] = 0;
    }
    vm->stack[i].pc = INTERNAL_ADDRESS_SIZE;
    vm->stack[i].acc = 0;
    vm->stack[i].ex = 0;
  }

  // SP
  vm->sp = 0;
  // BC size
  vm->bc_size = 0;
  //
  vm->sbp = 0;
  //
  vm->sd = 0;
  //
  vm->sp = 0;
}
/**
 *
 * @return ohvm*
 */
ohvm *new_ohvm()
{
  ohvm *openohvm = new_object(ohvm);
  // Init Vm
  init_ohvm(openohvm);
  // Init ram
  reset_ram(openohvm);
  //
  load_instrucsions(operate_functions);
  //
  return openohvm;
};
//
void run_bc(ohvm *vm)
{
  while (1)
  {
    sleep(1);
    execute(vm);
  }
}
//
void execute(ohvm *vm)
{
  uint32 deepth = vm->sd;
  uint32 i = vm->ram[vm->stack[deepth].pc];
  uint32 pc = vm->stack[deepth].pc;
  log_debug("CURRENT STACK:%d INSTRUCTION:0x%08x PC:%d", deepth, i, pc);
  operate_functions[vm->ram[pc]](vm);
}
// Load bytecode
void load_ohvmbc(char *path, ohvm *vm)
{
  log_debug("LOAD VM BYTE CODE");
  FILE *file = fopen(path, "rb");
  if (!file)
  {
    log_error("File not exist:%s!", path);
    exit(0);
  }
  else
  {
    fseek(file, 0L, SEEK_END);
    unsigned long bc_file_size = ftell(file);
    if (bc_file_size > MAX_RAM_SIZE - INTERNAL_ADDRESS_SIZE)
    {
      log_error("File too large!");
      exit(0);
    }
    fseek(file, 0, SEEK_SET);
    ohvmbc ohvmbc_info;
    fread(&ohvmbc_info, sizeof(ohvmbc), 1, file);
    ohvmbc_output(&ohvmbc_info);
    unsigned long bytecode_length = bc_file_size - type_size(ohvmbc_info);
    vm->bc_size = (uint32)bytecode_length;
    byte bytecode[bytecode_length];
    fseek(file, sizeof(ohvmbc), SEEK_SET);
    fread(bytecode, bytecode_length, 1, file);
    log_debug("BC FILE SIZE :%d BC SIZE:%d", bc_file_size, bytecode_length);
    for (uint32 i = INTERNAL_ADDRESS_SIZE;
         i < INTERNAL_ADDRESS_SIZE + bytecode_length; i++)
    {
      log_debug("RAM[%d] == [0X%04X]", i, bytecode[i - INTERNAL_ADDRESS_SIZE]);
      set_ram_value(i, bytecode[i - INTERNAL_ADDRESS_SIZE], vm);
    }
    fclose(file);
  }
}
// reset
void reset_ram(ohvm *vm)
{
  log_debug("RESET RAM");
  // [INTERNAL_ADDRESS_SIZE - MAX_RAM_SIZE]
  for (uint32 i = INTERNAL_ADDRESS_SIZE; i < MAX_RAM_SIZE; i++)
  {
    set_ram_value(i, 0, vm);
  }
}
// set value to ram
void set_ram_value(uint32 offset, byte value, ohvm *vm)
{
  if (offset > MAX_RAM_SIZE)
  {
    log_error("Offset too large ,max ram size is:%d", MAX_RAM_SIZE);
    exit(0);
  }
  // 1024 + offset
  // log_debug("SET RAM[%d] = 0X%02x", offset, value);
  vm->ram[offset] = value;
}
// get value from ram
uint32 get_ram_value(uint32 offset, ohvm *vm)
{

  // Max is 4
  if (offset > 4)
  {
    return 0;
  }
  if (offset == 1)
  {
    return (uint32)vm->ram[vm->stack[vm->sd].pc];
  }
  if (offset == 4)
  {
    union
    {
      byte bv[4];
      uint32 iv;
    } v, tv;
    tv.bv[0] = 1;
    if (tv.iv == 1)
    {
      // big
      for (uint32 i = 0; i < offset; i++)
      {
        v.bv[i] = vm->ram[vm->stack[vm->sd].pc + i];
      }
    }
    else
    {
      // little
      for (uint32 i = 0; i < offset; i++)
      {
        v.bv[offset - 1 - i] = vm->ram[vm->stack[vm->sd].pc + i];
      }
    }

    return v.iv;
  }
  return 0;
}
// set/get value to/from acc
void set_acc_value(uint32 value, ohvm *vm)
{
  uint32 deepth = vm->sd;
  vm->stack[deepth].acc = value;
  log_debug("SET STACK[%d], ACC = %d", deepth, value);
}
//
uint32 get_acc_value(ohvm *vm)
{
  uint32 deepth = vm->sd;
  return vm->stack[deepth].acc;
}
// set/get value to/from pc
void set_pc_value(uint32 value, ohvm *vm) {
  vm->stack[vm->sd].pc = value;
}
//
uint32 get_pc_value(ohvm *vm) {
  return vm->stack[vm->sd].pc;
}
// set/get value to/from sd
void set_sd(byte value, ohvm *vm) {
  vm->sd = value;
}
uint32 get_sd(ohvm *vm) {
  return vm->sd;
}
// set/get value to/from sp
void set_sp(byte value, ohvm *vm) {
  vm->sp = value;
}
uint32 get_sp(ohvm *vm) {
  return vm->sp;
}
// set/get value to/from ex
void set_ex(byte value, ohvm *vm) {
  vm->stack[vm->sd].ex = value;
}
//
uint32 get_ex(ohvm *vm) {
  return vm->stack[vm->sd].ex;
}
//
void set_r_value(int rn, uint32 value, ohvm *vm)
{
  vm->stack[vm->sd].r[rn] = value;
  log_debug("SET STACK[%d], R[%d] = %d", vm->sd, rn, value);
}
//
uint32 get_r_value(int rn, ohvm *vm)
{
  uint32 sd = vm->sd;
  stack stack = vm->stack[sd];
  return stack.r[rn];
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

// No operation
void NOP_0X00(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
// Start program
void START_0X01(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
// Stop vm
void STOP_0X02(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  exit(0);
}
// Reset all register
void RSTR_0X03(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
// SLEEP ms
void TIMER_0X04(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 value = get_ram_value(4, vm);
  sleep(value);
  set_pc_value(vm->stack[vm->sd].pc + 4, vm);
}
// GOTO
void GOTO_0X05(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);

  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 address = get_ram_value(4, vm);
  vm->stack[vm->sd].pc = address;
}
// CALL ADDR RETURN?
void CALL_0X06(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  if(vm->sd > MAX_STACK_DEEPTH){ 
      log_error("STACK OVERFLOW:%s", __FUNCTION__);
      exit(0);
  }
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 call_address = get_ram_value(4, vm);
  set_pc_value(vm->stack[vm->sd].pc + 4, vm);
  uint32 is_return = get_ram_value(1, vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  char covert_dest[10];
  int_to_string(covert_dest, call_address);
  int hex_address_value = hex_to_decimal(covert_dest);
  vm->sd = vm->sd + 1;
  set_pc_value(hex_address_value, vm);
  log_debug("CALL %d, %d, SD:%d", hex_address_value, is_return, vm->sd);
}
// Sub process return
void BACK_0X07(ohvm *vm) {
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  log_debug("CURRENT STACK DEEPTH:%d", vm->sd);
  // main: stack = 0
  if(vm->sd == 0){
    log_debug("PROGRAM FINISHED");
    exit(0);
  }
  uint32 current_acc_value = vm->stack[vm->sd].acc;
  vm->sd = vm->sd - 1;
  set_acc_value(vm->stack[vm->sd].acc + current_acc_value, vm);
}
// JMP == GOTO
void JMP_0X08(ohvm *vm) {
  GOTO_0X05(vm);
}
// Jump {address} if 0 == {Rn}
// else goto next instruction
// JZ {Rn} {Address} 
void JZ_0X09(ohvm *vm) {
  
}
// Jump {address} if ACC == {Rn}
// else goto next instruction
// JE {Rn} {Address} 
void JE_0X0A(ohvm *vm) {

}
// Jump {address} if EX == 1
// else goto next instruction
// JX {Address}
void JX_0X0B(ohvm *vm) {

}
//
void CMRAE_0X0C(ohvm *vm) {

}
//
void CMRAG_0X0D(ohvm *vm) {

}
//
void CMRAL_0X0E(ohvm *vm) {

}
//
void CMRRE_0X0F(ohvm *vm) {

}
//
void CMRRG_0X10(ohvm *vm) {

}
//
void CMRRL_0X11(ohvm *vm) {

}
//
void CMASE_0X12(ohvm *vm) {

}
//
void CMASG_0X13(ohvm *vm) {

}
//
void CMASL_0X14(ohvm *vm) {

}
//
void CMRSE_0X15(ohvm *vm) {

}
//
void CMRSG_0X16(ohvm *vm) {

}
//
void CMRSL_0X17(ohvm *vm) {

}
// Acc = Acc + 1
void INCA_0X18(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_acc_value(vm->stack[vm->sd].acc + 1, vm);
}
// Acc = Acc- 1
void DECA_0X19(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_acc_value(vm->stack[vm->sd].acc - 1, vm);
}
// Rn = Rn + 1
void INCR_0X1A(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  uint32 sd = vm->sd;
  stack stack = vm->stack[sd];
  set_r_value(rn, stack.r[rn] + 1, vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
// Rn = Rn -1
void DECR_0X1B(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  set_r_value(rn, vm->stack[vm->sd].r[rn] - 1, vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
// ADDAR Rn
void ADDAR_0X1C(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  uint32 rn_value = vm->stack[vm->sd].r[rn];
  uint32 acc = vm->stack[vm->sd].acc;
  set_acc_value(acc + rn_value, vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  log_debug("ADDAR: ACC=%d STACK[%d]R[%d] = %d", acc, vm->sd, rn, rn_value);
}
//
void SUBAR_0X1D(ohvm *vm) {

}
//
void INCS_0X1E(ohvm *vm) {

}
//
void DECS_0X1F(ohvm *vm) {

}
//
void ADDAS_0X20(ohvm *vm) {

}
//
void SUBAS_0X21(ohvm *vm) {

}
//
void ANDR_0X22(ohvm *vm) {

}
//
void AOR_0X23(ohvm *vm) {

}
//
void AXR_0X24(ohvm *vm) {

}
//
void BSLR_0X25(ohvm *vm) {

}
//
void BSRR_0X26(ohvm *vm) {

}
//
void BSLLR_0X27(ohvm *vm) {

}
//
void BSRLR_0X28(ohvm *vm) {

}
//
void ANDS_0X29(ohvm *vm) {

}
//
void AOS_0X2A(ohvm *vm) {

}
//
void AXS_0X2B(ohvm *vm) {

}
//
void BSLS_0X2C(ohvm *vm) {

}
//
void BSRS_0X2D(ohvm *vm) {

}
//
void BSLLS_0X2E(ohvm *vm) {

}
//
void BSRLS_0X2F(ohvm *vm) {

}
// IMA {value}
void IMA_0X30(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 value = get_ram_value(4, vm);
  set_acc_value(vm->stack[vm->sd].acc + value, vm);
  set_pc_value(vm->stack[vm->sd].pc + 4, vm);
}
// IMR {value}
void IMR_0X31(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  // PC +1 -> Rn
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  // PC +1 -> Value
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 value = get_ram_value(4, vm);
  // set value
  set_r_value(rn, value, vm);
  // Next
  set_pc_value(vm->stack[vm->sd].pc + 4, vm);
}
//
void IMS_0X32(ohvm *vm) {
  //TODO
}
// GET {address} -> ACC
void GET_0X33(ohvm *vm)
{
  log_debug("CURRENT EXECUTE FUNCTION:%s", __FUNCTION__);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
  uint32 rn = get_ram_value(1, vm);
  set_acc_value(vm->stack[vm->sd].acc + vm->stack[vm->sd].r[rn], vm);
  set_pc_value(vm->stack[vm->sd].pc + 1, vm);
}
void MVRR_0X34(ohvm *vm) {
  // TODO
}
void MVRS_0X35(ohvm *vm) {
  // TODO
}
void MVSR_0X36(ohvm *vm) {
  // TODO
}
void MVAR_0X37(ohvm *vm) {
  // TODO
}
void INTK_0X38(ohvm *vm) {}
void KEY_0X39(ohvm *vm) {}
void PLY_0X3A(ohvm *vm) {}
void PLYS_0X3B(ohvm *vm) {}
void GSET_0X3C(ohvm *vm) {}
void CLS_0X3D(ohvm *vm) {}
void DPXY_0X3E(ohvm *vm) {}
void DXY_0X3F(ohvm *vm) {}
void DCXY_0X40(ohvm *vm) {}
void DLINE_0X41(ohvm *vm) {}
void DRECT_0X42(ohvm *vm) {}
void SCRU_0X43(ohvm *vm) {}
void SCRD_0X44(ohvm *vm) {}
void SCRL_0X45(ohvm *vm) {}
void SCRR_0X46(ohvm *vm) {}
//=======================================================================
// Load Instructions
//=======================================================================
void load_instrucsions(operate_function *operate_functions)
{
  operate_functions[0X00] = NOP_0X00;
  operate_functions[0X01] = START_0X01;
  operate_functions[0X02] = STOP_0X02;
  operate_functions[0X03] = RSTR_0X03;
  operate_functions[0X04] = TIMER_0X04;
  operate_functions[0X05] = GOTO_0X05;
  operate_functions[0X06] = CALL_0X06;
  operate_functions[0X07] = BACK_0X07;
  operate_functions[0X08] = JMP_0X08;
  operate_functions[0X09] = JZ_0X09;
  operate_functions[0X0A] = JE_0X0A;
  operate_functions[0X0B] = JX_0X0B;
  operate_functions[0X0C] = CMRAE_0X0C;
  operate_functions[0X0D] = CMRAG_0X0D;
  operate_functions[0X0E] = CMRAL_0X0E;
  operate_functions[0X0F] = CMRRE_0X0F;
  operate_functions[0X10] = CMRRG_0X10;
  operate_functions[0X11] = CMRRL_0X11;
  operate_functions[0X12] = CMASE_0X12;
  operate_functions[0X13] = CMASG_0X13;
  operate_functions[0X14] = CMASL_0X14;
  operate_functions[0X15] = CMRSE_0X15;
  operate_functions[0X16] = CMRSG_0X16;
  operate_functions[0X17] = CMRSL_0X17;
  operate_functions[0X18] = INCA_0X18;
  operate_functions[0X19] = DECA_0X19;
  operate_functions[0X1A] = INCR_0X1A;
  operate_functions[0X1B] = DECR_0X1B;
  operate_functions[0X1C] = ADDAR_0X1C;
  operate_functions[0X1D] = SUBAR_0X1D;
  operate_functions[0X1E] = INCS_0X1E;
  operate_functions[0X1F] = DECS_0X1F;
  operate_functions[0X20] = ADDAS_0X20;
  operate_functions[0X21] = SUBAS_0X21;
  operate_functions[0X22] = ANDR_0X22;
  operate_functions[0X23] = AOR_0X23;
  operate_functions[0X24] = AXR_0X24;
  operate_functions[0X25] = BSLR_0X25;
  operate_functions[0X26] = BSRR_0X26;
  operate_functions[0X27] = BSLLR_0X27;
  operate_functions[0X28] = BSRLR_0X28;
  operate_functions[0X29] = ANDS_0X29;
  operate_functions[0X2A] = AOS_0X2A;
  operate_functions[0X2B] = AXS_0X2B;
  operate_functions[0X2C] = BSLS_0X2C;
  operate_functions[0X2D] = BSRS_0X2D;
  operate_functions[0X2E] = BSLLS_0X2E;
  operate_functions[0X2F] = BSRLS_0X2F;
  operate_functions[0X30] = IMA_0X30;
  operate_functions[0X31] = IMR_0X31;
  operate_functions[0X32] = IMS_0X32;
  operate_functions[0X33] = GET_0X33;
  operate_functions[0X34] = MVRR_0X34;
  operate_functions[0X35] = MVRS_0X35;
  operate_functions[0X36] = MVSR_0X36;
  operate_functions[0X37] = MVAR_0X37;
  operate_functions[0X38] = INTK_0X38;
  operate_functions[0X39] = KEY_0X39;
  operate_functions[0X3A] = PLY_0X3A;
  operate_functions[0X3B] = PLYS_0X3B;
  operate_functions[0X3C] = GSET_0X3C;
  operate_functions[0X3D] = CLS_0X3D;
  operate_functions[0X3E] = DPXY_0X3E;
  operate_functions[0X3F] = DXY_0X3F;
  operate_functions[0X40] = DCXY_0X40;
  operate_functions[0X41] = DLINE_0X41;
  operate_functions[0X42] = DRECT_0X42;
  operate_functions[0X43] = SCRU_0X43;
  operate_functions[0X44] = SCRD_0X44;
  operate_functions[0X45] = SCRL_0X45;
  operate_functions[0X46] = SCRR_0X46;
}

