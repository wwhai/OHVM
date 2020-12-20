#include <stdio.h>
unsigned char NOP = 0x0000;
unsigned char START = 0x0001;
unsigned char STOP = 0x0002;
unsigned char RSTR = 0x0003;
unsigned char TIMER = 0x0004;
unsigned char GOTO = 0x0005;
unsigned char CALL = 0x0006;
unsigned char BACK = 0x0007;
unsigned char JMP = 0x0008;
unsigned char JZ = 0x0009;
unsigned char JE = 0x000A;
unsigned char JX = 0x000B;
unsigned char CMRAE = 0x000C;
unsigned char CMRAG = 0x000D;
unsigned char CMRAL = 0x000E;
unsigned char CMRRE = 0x000F;
unsigned char CMRRG = 0x0010;
unsigned char CMRRL = 0x0011;
unsigned char CMASE = 0x0012;
unsigned char CMASG = 0x0013;
unsigned char CMASL = 0x0014;
unsigned char CMRSE = 0x0015;
unsigned char CMRSG = 0x0016;
unsigned char CMRSL = 0x0017;
unsigned char INCA = 0x0018;
unsigned char DECA = 0x0019;
unsigned char INCR = 0x001A;
unsigned char DECR = 0x001B;
unsigned char ADDAR = 0x001C;
unsigned char SUBAR = 0x001D;
unsigned char INCS = 0x001E;
unsigned char DECS = 0x001F;
unsigned char ADDAS = 0x0020;
unsigned char SUBAS = 0x0021;
unsigned char ANDR = 0x0022;
unsigned char AOR = 0x0023;
unsigned char AXR = 0x0024;
unsigned char BSLR = 0x0025;
unsigned char BSRR = 0x0026;
unsigned char BSLLR = 0x0027;
unsigned char BSRLR = 0x0028;
unsigned char ANDS = 0x0029;
unsigned char AOS = 0x002A;
unsigned char AXS = 0x002B;
unsigned char BSLS = 0x002C;
unsigned char BSRS = 0x002D;
unsigned char BSLLS = 0x002E;
unsigned char BSRLS = 0x002F;
unsigned char IMA = 0x0030;
unsigned char IMR = 0x0031;
unsigned char IMS = 0x0032;
unsigned char GET = 0x0033;
unsigned char MVRR = 0x0034;
unsigned char MVRS = 0x0035;
unsigned char MVSR = 0x0036;
unsigned char MVAR = 0x0037;
unsigned char INTK = 0x0038;
unsigned char KEY = 0x0039;
unsigned char PLY = 0x003A;
unsigned char PLYS = 0x003B;
unsigned char GSET = 0x003C;
unsigned char CLS = 0x003D;
unsigned char DPXY = 0x003E;
unsigned char DXY = 0x003F;
unsigned char DCXY = 0x0040;
unsigned char DLINE = 0x0041;
unsigned char DRECT = 0x0042;
unsigned char SCRU = 0x0043;
unsigned char SCRD = 0x0044;
unsigned char SCRL = 0x0045;
unsigned char SCRR = 0x0046;
/**
 * @brief 
 * 
 * 
 * 
 */
void print_vm_state()
{
    printf("┏━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ F1 ┃ F2 ┃ F3 ┃ F4 ┃ F5 ┃ F6 ┃ F7 ┃ F8 ┃        START ADDRESS    ┃\n");
    printf("┃━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    printf("┃ %-2d ┃ %-2d ┃ %-2d ┃ %-2d ┃ %-2d ┃ %-2d ┃ %-2d ┃ %-2d ┃   0X%016xH   ┃\n", 0, 0, 0, 0, 0, 0, 0, 0, 0);
    printf("┣━━━━┻━━━━╋━━━━┻━━━━┻━━━━┻━━━━┻━━━━┻━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
    printf("┃ PC      ┃     0X%016xH        ┃  %-8s            ┃\n", 0, "NOP");
    printf("┃━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    printf("┃ ACC     ┃                   0X%016xH                 ┃\n", 0);
    printf("┃━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    printf("┃ SD      ┃                   0X%016xH                 ┃\n", 0);
    printf("┃━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    printf("┃ SP      ┃                   0X%016xH                 ┃\n", 0);
    printf("┃━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    printf("┃ EX      ┃                   0X%016xH                 ┃\n", 0);
    printf("┗━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}
/**
 * @brief 
 * 
 */
void p()
{
    for (int i = 0x3c; i < 0x47; i++)
    {
        printf("0X%08XH\n", i);
    }
}
int compile_bc(unsigned char *bc, int length)
{
    FILE *file = fopen("test_nop.ovmbc", "wb");
    if (!file)
    {
        printf("Unable to open file!");
        return 1;
    }
    unsigned char v1 = 0, v2 = 0, v3 = 1, end = 0x0002;
    unsigned int start_address = 0;
    fwrite("VMBC", 4, 1, file);
    fwrite(&v1, 1, 1, file);
    fwrite(&v2, 1, 1, file);
    fwrite(&v3, 1, 1, file);
    fwrite(&start_address, 4, 1, file);
    fwrite(bc, length, 1, file);
    fwrite(&end, 1, 1, file);
    fclose(file);
    return 0;
}
// rm -rf ./test_nop.ovmbc && gcc test.c -o test && ./test && rm -rf test && xxd test_nop.ovmbc
int main(int argc, char const *argv[])
{
    unsigned char bc[] = {IMA,
                          (unsigned char)0,
                          (unsigned char)0,
                          (unsigned char)0,
                          (unsigned char)1,
                          INCA,
                          INCA};
    compile_bc(bc, sizeof(bc));
    return 0;
}
