#include "inst.h"

void
env16_nop(Env16Machine* env16) {

    env16->cpu.PC += 1;

}

void
env16_mov(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC] + 1;

    WORD temp;
    WORD* r1 = (WORD*) &env16->cpu.rSTART + LO_NIBBLE(*p);
    WORD* r2 = (WORD*) &env16->cpu.rSTART + HI_NIBBLE(*p);

    *r1 = *r2;

    env16->cpu.PC += 2;

}

void
env16_lda(Env16Machine* env16) {

    env16->cpu.rA = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += 3;

}

void
env16_ldb(Env16Machine* env16) {

    env16->cpu.rB = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += 3;

}

void
env16_ldc(Env16Machine* env16) {

    env16->cpu.rC = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += 3;

}

void
env16_ldd(Env16Machine* env16) {

    env16->cpu.rD = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += 3;

}

void
env16_lde(Env16Machine* env16) {

    env16->cpu.rE = *(WORD*) (&env16->ram[env16->cpu.PC] + 1);

    env16->cpu.PC += 3;

}

void
env16_str(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE op = LO_NIBBLE(*p);

    if (op >= 8) {

        *(WORD*) &env16->ram[*(&env16->cpu.rSTART + (op - 0x8))] = env16->cpu.rA;

    }
    else {

        *(WORD*) &env16->ram[*(WORD*)(++p)] = env16->cpu.rA;

    }

    env16->cpu.PC += 3;

}

void
env16_ldr(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE op = LO_NIBBLE(*p);

    if (op >= 8) {
        env16->cpu.rA = *(WORD*) env16->ram + *(&env16->cpu.rSTART + (op - 0x8));
    }
    else {
        env16->cpu.rA = *(WORD*) env16->ram + *(WORD*)(++p);
    }

    env16->cpu.PC += 3;

}

void
env16_jmp(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE op = LO_NIBBLE(*p);

    if (op >= 8 && op <= 13) {
        env16->cpu.PC = *(&env16->cpu.rSTART + (op - 0x8));
    }
    else {
        env16->cpu.PC = *(WORD*)(p+1);
    }

}

void
env16_jpz(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC];

    BYTE op = LO_NIBBLE(*p);

    if (env16->cpu.rA == 0) {
        if (op >= 8 && op <= 13) {
            env16->cpu.PC = *(&env16->cpu.rSTART + (op - 0x8));
        }
        else {
            env16->cpu.PC = *(WORD*)(p+1);
        }
    }
    else {
        env16->cpu.PC += 3;
    }

}

void
env16_add(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC] + 1;
    WORD n1 = *(&env16->cpu.rSTART + LO_NIBBLE(*p));
    WORD n2 = *(&env16->cpu.rSTART + HI_NIBBLE(*p));

    env16->cpu.rA = n1 + n2;

    env16->cpu.PC += 2;

}

void
env16_sub(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC] + 1;
    WORD n1 = *(&env16->cpu.rSTART + LO_NIBBLE(*p));
    WORD n2 = *(&env16->cpu.rSTART + HI_NIBBLE(*p));

    env16->cpu.rA = n1 - n2;

    env16->cpu.PC += 2;

}

void
env16_div(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC] + 1;
    WORD n1 = *(&env16->cpu.rSTART + LO_NIBBLE(*p));
    WORD n2 = *(&env16->cpu.rSTART + HI_NIBBLE(*p));

    env16->cpu.rA = n1 / n2;

    env16->cpu.PC += 2;

}

void
env16_mul(Env16Machine* env16) {

    BYTE* p = &env16->ram[env16->cpu.PC] + 1;
    WORD n1 = *(&env16->cpu.rSTART + LO_NIBBLE(*p));
    WORD n2 = *(&env16->cpu.rSTART + HI_NIBBLE(*p));

    env16->cpu.rA = n1 * n2;

    env16->cpu.PC += 2;

}

void
env16_hlt(Env16Machine* env16) {

    running = false;

    env16->cpu.PC += 1;

    printf("\n[HLT]\n");

}