//
//  ohvmcc.c
//  OHVM
//
//  Created by 王文海 on 2020/12/24.
//

#include "ohvmcc.h"
typedef unsigned char I;

unsigned char _NOP = 0x0000;
unsigned char _START = 0x0001;
unsigned char _STOP = 0x0002;
unsigned char _RSTR = 0x0003;
unsigned char _TIMER = 0x0004;
unsigned char _GOTO = 0x0005;
unsigned char _CALL = 0x0006;
unsigned char _BACK = 0x0007;
unsigned char _JMP = 0x0008;
unsigned char _JZ = 0x0009;
unsigned char _JE = 0x000A;
unsigned char _JX = 0x000B;
unsigned char _CMRAE = 0x000C;
unsigned char _CMRAG = 0x000D;
unsigned char _CMRAL = 0x000E;
unsigned char _CMRRE = 0x000F;
unsigned char _CMRRG = 0x0010;
unsigned char _CMRRL = 0x0011;
unsigned char _CMASE = 0x0012;
unsigned char _CMASG = 0x0013;
unsigned char _CMASL = 0x0014;
unsigned char _CMRSE = 0x0015;
unsigned char _CMRSG = 0x0016;
unsigned char _CMRSL = 0x0017;
unsigned char _INCA = 0x0018;
unsigned char _DECA = 0x0019;
unsigned char _INCR = 0x001A;
unsigned char _DECR = 0x001B;
unsigned char _ADDAR = 0x001C;
unsigned char _SUBAR = 0x001D;
unsigned char _INCS = 0x001E;
unsigned char _DECS = 0x001F;
unsigned char _ADDAS = 0x0020;
unsigned char _SUBAS = 0x0021;
unsigned char _ANDR = 0x0022;
unsigned char _AOR = 0x0023;
unsigned char _AXR = 0x0024;
unsigned char _BSLR = 0x0025;
unsigned char _BSRR = 0x0026;
unsigned char _BSLLR = 0x0027;
unsigned char _BSRLR = 0x0028;
unsigned char _ANDS = 0x0029;
unsigned char _AOS = 0x002A;
unsigned char _AXS = 0x002B;
unsigned char _BSLS = 0x002C;
unsigned char _BSRS = 0x002D;
unsigned char _BSLLS = 0x002E;
unsigned char _BSRLS = 0x002F;
unsigned char _IMA = 0x0030;
unsigned char _IMR = 0x0031;
unsigned char _IMS = 0x0032;
unsigned char _GET = 0x0033;
unsigned char _MVRR = 0x0034;
unsigned char _MVRS = 0x0035;
unsigned char _MVSR = 0x0036;
unsigned char _MVAR = 0x0037;
unsigned char _INTK = 0x0038;
unsigned char _KEY = 0x0039;
unsigned char _PLY = 0x003A;
unsigned char _PLYS = 0x003B;
unsigned char _GSET = 0x003C;
unsigned char _CLS = 0x003D;
unsigned char _DPXY = 0x003E;
unsigned char _DXY = 0x003F;
unsigned char _DCXY = 0x0040;
unsigned char _DLINE = 0x0041;
unsigned char _DRECT = 0x0042;
unsigned char _SCRU = 0x0043;
unsigned char _SCRD = 0x0044;
unsigned char _SCRL = 0x0045;
unsigned char _SCRR = 0x0046;
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

//gcc oohvmcc.c -o oohvmcc && ./oohvmcc && rm -rf oohvmcc && xxd oohvmcc_nop.oohvmbc && mv ./*.oohvmbc ./res/
int main2(int argc, char const *argv[])
{
    // NOP
    unsigned char nop_test[] = {
        _NOP,
        _NOP,
        _NOP,
        // assert pc == 3
    };
    compile_bc("nop_test.oohvmbc", nop_test, sizeof(nop_test));
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
    compile_bc("add_r0_r1_test.oohvmbc", add_r0_r1_test, sizeof(add_r0_r1_test));
    return 0;
}
