//
//  ohvm.h
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#ifndef ohvm_h
#define ohvm_h

#include <stdio.h>
#include <stdlib.h>
#include "clog.h"
#include "cpu.h"
#include "types.h"
// Max byte code size, Develop use 4kb, Can configure other value
#ifndef MAX_RAM_SIZE
#define MAX_RAM_SIZE 1024 * 4
#endif
// Max stack deepth
#define MAX_STACK_DEEPTH 16
// Max register count
#define MAX_REGISTER_COUNT 8
// Internal address
#define INTERNAL_ADDRESS_SIZE 1024
// INSTRUCTIONS_COUNT: 256
#define INSTRUCTIONS_COUNT 0xFF
typedef struct __attribute__((__packed__))
{
  uint32 r[MAX_REGISTER_COUNT];
  uint32 pc;
  uint32 acc;
  uint32 ex;
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
} stack;
/**
* VM
*/
typedef struct __attribute__((__packed__))
{
  // Bytecode size
  uint32 bc_size;
  // stack deepth
  uint32 sd;
  // stack pointer
  uint32 sp;
  // stack base pointer
  uint32 sbp;
  // RAM
  byte ram[MAX_RAM_SIZE];
  // stack 1024 * 8
  stack stack[MAX_STACK_DEEPTH];
} ohvm;
// operate function define
typedef void (*operate_function)(ohvm *vm);
// new ohvm
ohvm *new_ohvm(void);
// Initial
void init_ohvm(ohvm *vm);
// load byte code
void load_ohvmbc(char *path, ohvm *vm);
// run byte code
void run_bc(ohvm *vm);
// set/get value to ram
void set_ram_value(uint32 offset, byte value, ohvm *vm);
uint32 get_ram_value(uint32 offset, ohvm *vm);
// reset
void reset_ram(ohvm *vm);
// set/get value to/from acc
void set_acc_value(uint32 value, ohvm *vm);
uint32 get_acc_value(ohvm *vm);
// set/get value to/from pc
void set_pc_value(uint32 value, ohvm *vm);
uint32 get_pc_value(ohvm *vm);
// set/get value to/from sd
void set_sd(byte value, ohvm *vm);
uint32 get_sd(ohvm *vm);
// set/get value to/from sp
void set_sp(byte value, ohvm *vm);
uint32 get_sp(ohvm *vm);
// set/get value to/from ex
void set_ex(byte value, ohvm *vm);
uint32 get_ex(ohvm *vm);
// set/get register value
void set_r_value(int rn, uint32 value, ohvm *vm);
uint32 get_r_value(int rn, ohvm *vm);
//
void execute(ohvm *vm);
//
void load_instrucsions(operate_function *operate_functions);
//
void NOP_0X0000(ohvm *vm);
void START_0X0001(ohvm *vm);
void STOP_0X0002(ohvm *vm);
void RSTR_0X0003(ohvm *vm);
void TIMER_0X0004(ohvm *vm);
void GOTO_0X0005(ohvm *vm);
void CALL_0X0006(ohvm *vm);
void BACK_0X0007(ohvm *vm);
void JMP_0X0008(ohvm *vm);
void JZ_0X0009(ohvm *vm);
void JE_0X000A(ohvm *vm);
void JX_0X000B(ohvm *vm);
void CMRAE_0X000C(ohvm *vm);
void CMRAG_0X000D(ohvm *vm);
void CMRAL_0X000E(ohvm *vm);
void CMRRE_0X000F(ohvm *vm);
void CMRRG_0X0010(ohvm *vm);
void CMRRL_0X0011(ohvm *vm);
void CMASE_0X0012(ohvm *vm);
void CMASG_0X0013(ohvm *vm);
void CMASL_0X0014(ohvm *vm);
void CMRSE_0X0015(ohvm *vm);
void CMRSG_0X0016(ohvm *vm);
void CMRSL_0X0017(ohvm *vm);
void INCA_0X0018(ohvm *vm);
void DECA_0X0019(ohvm *vm);
void INCR_0X001A(ohvm *vm);
void DECR_0X001B(ohvm *vm);
void ADDAR_0X001C(ohvm *vm);
void SUBAR_0X001D(ohvm *vm);
void INCS_0X001E(ohvm *vm);
void DECS_0X001F(ohvm *vm);
void ADDAS_0X0020(ohvm *vm);
void SUBAS_0X0021(ohvm *vm);
void ANDR_0X0022(ohvm *vm);
void AOR_0X0023(ohvm *vm);
void AXR_0X0024(ohvm *vm);
void BSLR_0X0025(ohvm *vm);
void BSRR_0X0026(ohvm *vm);
void BSLLR_0X0027(ohvm *vm);
void BSRLR_0X0028(ohvm *vm);
void ANDS_0X0029(ohvm *vm);
void AOS_0X002A(ohvm *vm);
void AXS_0X002B(ohvm *vm);
void BSLS_0X002C(ohvm *vm);
void BSRS_0X002D(ohvm *vm);
void BSLLS_0X002E(ohvm *vm);
void BSRLS_0X002F(ohvm *vm);
void IMA_0X0030(ohvm *vm);
void IMR_0X0031(ohvm *vm);
void IMS_0X0032(ohvm *vm);
void GET_0X0033(ohvm *vm);
void MVRR_0X0034(ohvm *vm);
void MVRS_0X0035(ohvm *vm);
void MVSR_0X0036(ohvm *vm);
void MVAR_0X0037(ohvm *vm);
void INTK_0X0038(ohvm *vm);
void KEY_0X0039(ohvm *vm);
void PLY_0X003A(ohvm *vm);
void PLYS_0X003B(ohvm *vm);
void GSET_0X003C(ohvm *vm);
void CLS_0X003D(ohvm *vm);
void DPXY_0X003E(ohvm *vm);
void DXY_0X003F(ohvm *vm);
void DCXY_0X0040(ohvm *vm);
void DLINE_0X0041(ohvm *vm);
void DRECT_0X0042(ohvm *vm);
void SCRU_0X0043(ohvm *vm);
void SCRD_0X0044(ohvm *vm);
void SCRL_0X0045(ohvm *vm);
void SCRR_0X0046(ohvm *vm);

#endif
