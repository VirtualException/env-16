#include "inst.h"

WORD
env16_nop(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.PC += 1;
    return 1;
}

WORD
env16_swt(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    BYTE* p = &CURR_BYTE(env16);
    p++;

    WORD temp;
    WORD* n1 = &env16->cpu.rSTART + LO_NIBBLE(*p);
    WORD* n2 = &env16->cpu.rSTART + HI_NIBBLE(*p);

    temp = *n1;
    *n1 = *n2;
    *n2 = temp;

    env16->cpu.PC += 2;
    return 3;
}

WORD
env16_lda(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.rA = *(WORD*) (&CURR_BYTE(env16) + 1);

    env16->cpu.PC += 3;
    return 1;
}

WORD
env16_ldb(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.rB = *(WORD*) (&CURR_BYTE(env16) + 1);

    env16->cpu.PC += 3;
    return 1;
}

WORD
env16_ldc(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.rC = *(WORD*) (&CURR_BYTE(env16) + 1);

    env16->cpu.PC += 3;
    return 1;
}
WORD
env16_ldd(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.rD = *(WORD*) (&CURR_BYTE(env16) + 1);

    env16->cpu.PC += 3;
    return 1;
}

WORD
env16_lde(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.rE = *(WORD*) (&CURR_BYTE(env16) + 1);

    env16->cpu.PC += 3;
    return 1;
}

WORD
env16_str(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    BYTE* p = &CURR_BYTE(env16);

    BYTE op = LO_NIBBLE(*p);

    if (op >= 8) {

        *(WORD*) &env16->ram[*(&env16->cpu.rSTART + (int) (op - 0x8))] = env16->cpu.rA;

    }
    else {

        *(WORD*) &env16->ram[*(WORD*) p++] = env16->cpu.rA;

    }

    env16->cpu.PC += 3;
    return 1;
}

WORD
env16_ldr(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.PC += 3;
    return 1;
}

WORD
env16_jmp(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.PC += 3;
    return 1;
}

WORD
env16_jpz(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.PC += 3;
    return 2;
}

WORD
env16_add(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    BYTE p = CURR_BYTE(env16);
    WORD n1 = env16->cpu.rSTART + LO_NIBBLE(p);
    WORD n2 = env16->cpu.rSTART + HI_NIBBLE(p);

    env16->cpu.rA = n1 + n2;

    env16->cpu.PC += 2;
    return 3;
}

WORD
env16_sub(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.PC += 2;
    return 3;
}

WORD
env16_div(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.PC += 2;
    return 3;
}

WORD
env16_mul(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    env16->cpu.PC += 2;
    return 3;
}

WORD
env16_hlt(Env16Machine* env16) {

#if _ENV16DBG
    env16->cpu._DBG_CURRENT_OPCODE = HI_NIBBLE(CURR_BYTE(env16));
#endif

    running = false;

    env16->cpu.CYCLE += 1;
    return 1;
}