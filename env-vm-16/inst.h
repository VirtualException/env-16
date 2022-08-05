#ifndef _INST_H
#define _INST_H

#include "env16.h"

WORD env16_nop(Env16Machine* env16);

WORD env16_swt(Env16Machine* env16);

WORD env16_lda(Env16Machine* env16);
WORD env16_ldb(Env16Machine* env16);
WORD env16_ldc(Env16Machine* env16);
WORD env16_ldd(Env16Machine* env16);
WORD env16_lde(Env16Machine* env16);

WORD env16_str(Env16Machine* env16);
WORD env16_ldr(Env16Machine* env16);

WORD env16_jmp(Env16Machine* env16);
WORD env16_jpz(Env16Machine* env16);

WORD env16_add(Env16Machine* env16);
WORD env16_sub(Env16Machine* env16);
WORD env16_div(Env16Machine* env16);
WORD env16_mul(Env16Machine* env16);

WORD env16_hlt(Env16Machine* env16);

#endif