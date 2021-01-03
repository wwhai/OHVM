//
//  utils.c
//  OHVM
//
//  Created by wwhai on 2020/12/22.
//

#include "utils.h"
#include<string.h>

int hex_to_decimal(char hexVal[])
{
    int len = strlen(hexVal);
    int base = 1;
    int dec_val = 0;
    for (int i=len-1; i>=0; i--)
    {
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            dec_val += (hexVal[i] - 48)*base;
            base = base * 16;
        }
        else if (hexVal[i]>='A' && hexVal[i]<='F')
        {
            dec_val += (hexVal[i] - 55)*base;
            base = base*16;
        }
    }
    return dec_val;
}
//
void int_to_string(char dest[], int number){
    char temp[10];
    sprintf(temp, "%x", number);
    if (temp[0] == '0' && (temp[1] == 'x' || temp[1] == 'X'))
    {
        sprintf(dest, "%x", number);
    }
    else{
        sprintf(dest, "0x%x", number);
    }
}

