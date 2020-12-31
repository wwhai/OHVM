//
//  ohvmcc.c
//  OHVM
//
//  Created by 王文海 on 2020/12/24.
//

#include "ohvmcc.h"
typedef unsigned char I;

unsigned char _NOP = 0x00;
unsigned char _START = 0x01;
unsigned char _STOP = 0x02;
unsigned char _RSTR = 0x03;
unsigned char _TIMER = 0x04;
unsigned char _GOTO = 0x05;
unsigned char _CALL = 0x06;
unsigned char _BACK = 0x07;
unsigned char _JMP = 0x08;
unsigned char _JZ = 0x09;
unsigned char _JE = 0x0A;
unsigned char _JX = 0x0B;
unsigned char _CMRAE = 0x0C;
unsigned char _CMRAG = 0x0D;
unsigned char _CMRAL = 0x0E;
unsigned char _CMRRE = 0x0F;
unsigned char _CMRRG = 0x10;
unsigned char _CMRRL = 0x11;
unsigned char _CMASE = 0x12;
unsigned char _CMASG = 0x13;
unsigned char _CMASL = 0x14;
unsigned char _CMRSE = 0x15;
unsigned char _CMRSG = 0x16;
unsigned char _CMRSL = 0x17;
unsigned char _INCA = 0x18;
unsigned char _DECA = 0x19;
unsigned char _INCR = 0x1A;
unsigned char _DECR = 0x1B;
unsigned char _ADDAR = 0x1C;
unsigned char _SUBAR = 0x1D;
unsigned char _INCS = 0x1E;
unsigned char _DECS = 0x1F;
unsigned char _ADDAS = 0x20;
unsigned char _SUBAS = 0x21;
unsigned char _ANDR = 0x22;
unsigned char _AOR = 0x23;
unsigned char _AXR = 0x24;
unsigned char _BSLR = 0x25;
unsigned char _BSRR = 0x26;
unsigned char _BSLLR = 0x27;
unsigned char _BSRLR = 0x28;
unsigned char _ANDS = 0x29;
unsigned char _AOS = 0x2A;
unsigned char _AXS = 0x2B;
unsigned char _BSLS = 0x2C;
unsigned char _BSRS = 0x2D;
unsigned char _BSLLS = 0x2E;
unsigned char _BSRLS = 0x2F;
unsigned char _IMA = 0x30;
unsigned char _IMR = 0x31;
unsigned char _IMS = 0x32;
unsigned char _GET = 0x33;
unsigned char _MVRR = 0x34;
unsigned char _MVRS = 0x35;
unsigned char _MVSR = 0x36;
unsigned char _MVAR = 0x37;
unsigned char _INTK = 0x38;
unsigned char _KEY = 0x39;
unsigned char _PLY = 0x3A;
unsigned char _PLYS = 0x3B;
unsigned char _GSET = 0x3C;
unsigned char _CLS = 0x3D;
unsigned char _DPXY = 0x3E;
unsigned char _DXY = 0x3F;
unsigned char _DCXY = 0x40;
unsigned char _DLINE = 0x41;
unsigned char _DRECT = 0x42;
unsigned char _SCRU = 0x43;
unsigned char _SCRD = 0x44;
unsigned char _SCRL = 0x45;
unsigned char _SCRR = 0x46;
//
int compile_bc(char *name, unsigned char *bc, int length)
{
    FILE *file = fopen(name, "wb");
    if (!file)
    {
        printf("Unable to open file!");
        return 1;
    }
    unsigned int start = 0;
    unsigned char v1 = 0, v2 = 0, v3 = 1, stop = 0x0002;
    fwrite("VMBC", 4, 1, file);
    printf("===> Write file header 'VMBC'\n");
    fwrite(&v1, 1, 1, file);
    fwrite(&v2, 1, 1, file);
    fwrite(&v3, 1, 1, file);
    printf("===> Write file version %d.%d.%d\n", v1, v2, v3);
    fwrite(&start, 4, 1, file);
    printf("===> Start address is: %d\n", start);
    fwrite(bc, length, 1, file);
    fwrite(&stop, 1, 1, file);
    fclose(file);
    return 0;
}

//gcc oohvmcc.c -o oohvmcc && ./oohvmcc && rm -rf oohvmcc && xxd oohvmcc_nop.ohvmbc && mv ./*.ohvmbc ./res/
int main(int argc, char const *argv[])
{
    // NOP
    unsigned char nop_test[] = {
        _NOP,
        _NOP,
        _NOP,
        // assert pc == 3
    };
    compile_bc("nop_test.ohvmbc", nop_test, sizeof(nop_test));
    // ADD R0 R1
    unsigned char add_r0_r1_test[] = {
        // IMR R0 0001H
        _IMR,
        (I)0x0000, // R0
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        // IMR R1 0001H
        _IMR,
        (I)0x0001, // R1
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        (I)0x0001,
        // IMR R2 0002H
        _IMR,
        (I)0x0002, // R2
        (I)0x0000,
        (I)0x0000,
        (I)0x0000,
        (I)0x0002,
        _ADDAR,
        (I)0x0000, // ADDAR R0
        _ADDAR,
        (I)0x0001, // ADDAR R1
        _ADDAR,
        (I)0x0002, // ADDAR R2
        // assert ACC == 3
    };
    compile_bc("add_r0_r1_test.ohvmbc", add_r0_r1_test, sizeof(add_r0_r1_test));
    unsigned char call_stack_overflow_test[] = {
      _IMA,       // Start at 0x0001 (ram[2015])
       (I)0x00,
       (I)0x00,
       (I)0x00,
       (I)0x01,
      _ADDAR,
       (I)0x00,   // ADDAR R0
      _CALL,      // CALL 0x0001 BACK
       (I)0x00,
       (I)0x00,
       (I)0x04,
       (I)0x01,
       (I)0x01,
    };
    compile_bc("call_stack_overflow_test.ohvmbc", call_stack_overflow_test, sizeof(call_stack_overflow_test));
    unsigned char call_test[] = {
       _CALL,      // CALL 0x0001 BACK
           (I)0x00,
           (I)0x00,
           (I)0x04,
           (I)0x07,
           (I)0x01,
           (I)0x02,
      //----------------------------
      // FUN:
      //    ADD A R0
      //    BACK
      // END
      _IMR,// 0x31     // Start at 0x0001 (ram[2015])
           (I)0x00,    // Init R0 = 0
           (I)0x00,
           (I)0x00,
           (I)0x00,
           (I)0x01,
      _ADDAR,          // 1C ,  0x1C
           (I)0x00,
           (I)0x07,    // RETURN
      //----------------------------
   };
    compile_bc("call_test.ohvmbc", call_test, sizeof(call_test));
    return 0;
}
