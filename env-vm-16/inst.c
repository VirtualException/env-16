#include "inst.h"

void
env16_nop(Env16Machine* env16) {

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_mov(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    *(&env16->cpu.rSTART + *++p) = *(&env16->cpu.rSTART + *++p);

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_lda(Env16Machine* env16) {

    env16->cpu.rA = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_ldb(Env16Machine* env16) {

    env16->cpu.rB = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_ldc(Env16Machine* env16) {

    env16->cpu.rC = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_ldd(Env16Machine* env16) {

    env16->cpu.rD = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_lde(Env16Machine* env16) {

    env16->cpu.rE = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_str(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE r = LO_NIBBLE(*p);

    if (r >= 8 && r <= 13) {
        *(WORD*) &env16->ram[*(&env16->cpu.rSTART + (r - 0x8))] = (WORD) env16->cpu.rA;
    }
    else {

        *(WORD*) &env16->ram[*(WORD*) ++p] = (WORD) env16->cpu.rA; // 16-bit? 8-bit? LGJAOSURGHOUH!

    }

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_ldr(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE r = LO_NIBBLE(*p);

    if (r >= 8 && r <= 13) {
        env16->cpu.rA = *(BYTE*) env16->ram + *(&env16->cpu.rSTART + (r - 0x8));
    }
    else {
        env16->cpu.rA = *(BYTE*) env16->ram + *(WORD*)(++p);
        printf("0x%X ", *(BYTE*) env16->ram + *(WORD*)(p));
        printf("0x%X ", *(WORD*)(p));
    }

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_jmp(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE r = LO_NIBBLE(*p);

    if (r >= 8 && r <= 13) {
        env16->cpu.PC = *(&env16->cpu.rSTART + (r - 0x8));
    }
    else {
        env16->cpu.PC = *(WORD*)(p+1);
    }

}

void
env16_jpz(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE r = LO_NIBBLE(*p);

    if (env16->cpu.rA == 0) {
        if (r >= 8 && r <= 13) {
            env16->cpu.PC = *(&env16->cpu.rSTART + (r - 0x8));
        }
        else {
            env16->cpu.PC = *(WORD*) (p+1);
        }
    }
    else {
        env16->cpu.PC += ENV16_INST_BYTE_LEN;
    }

}

void
env16_add(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];
    WORD n1 = *(&env16->cpu.rSTART + *++p);
    WORD n2 = *(&env16->cpu.rSTART + *++p);

    env16->cpu.rA = n1 + n2;

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_sub(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];
    WORD n1 = *(&env16->cpu.rSTART + *++p);
    WORD n2 = *(&env16->cpu.rSTART + *++p);

    env16->cpu.rA = n2 - n1;

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_div(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];
    WORD n1 = *(&env16->cpu.rSTART + *++p);
    WORD n2 = *(&env16->cpu.rSTART + *++p);

    env16->cpu.rA = n1 / n2;

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_mul(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];
    WORD n1 = *(&env16->cpu.rSTART + *++p);
    WORD n2 = *(&env16->cpu.rSTART + *++p);

    env16->cpu.rA = n1 * n2;

    env16->cpu.PC += ENV16_INST_BYTE_LEN;

}

void
env16_hlt(Env16Machine* env16) {

    running = false;

    printf("\n[HLT]\n");

}