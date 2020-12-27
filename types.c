//
//  types.c
//  OpenVM
//
//  Created by wwhai on 2020/11/25.
//

#include "types.h"
void ohvmbc_output(ohvmbc *bc)
{
    log_debug("File type:%s Version:%d-%d-%d Start at:0x%08xH",
              bc->type, bc->v_main, bc->v_progress, bc->v_bugfix, bc->start_address);
}
