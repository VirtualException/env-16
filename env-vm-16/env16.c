#include <pthread.h>
#include <memory.h>

#include "env16.h"
#include "inst.h"
#include "dpy.h"

Env16Cpu* dump_cpu;

bool running = true;
bool winrunning = true;

void
ctrlcHandler(int i) {

    running = false;
    winrunning = false;

}

void
dumpReg() {

    printf("\n[ENV-VM-16 DUMP]\n"
            "rA: %d (0x%X)\nrB: %d (0x%X)\nrC: %d (0x%X)\nrD: %d (0x%X)\nrE: %d (0x%X)\n"
            "PC: %d (0x%X), Opcode: %d (0x%X)\n",
            dump_cpu->rA, dump_cpu->rA, dump_cpu->rB, dump_cpu->rB, dump_cpu->rC, dump_cpu->rC, dump_cpu->rD, dump_cpu->rD, dump_cpu->rE, dump_cpu->rE,
            dump_cpu->PC, dump_cpu->PC, dump_cpu->CURRENT_OPCODE, dump_cpu->CURRENT_OPCODE);

}

int
initEnv16(Env16Machine* env16, FILE** exf) {

    memset(&env16->cpu, 0x0, sizeof(Env16Cpu));

    env16->ram = malloc(ENV16_MEMBYTES);
    memset(env16->ram, 0x0, ENV16_MEMBYTES);

    env16->inst[0x0] = env16_nop;
    env16->inst[0x1] = env16_mov;
    env16->inst[0x2] = env16_lda;
    env16->inst[0x3] = env16_ldb;
    env16->inst[0x4] = env16_ldc;
    env16->inst[0x5] = env16_ldd;
    env16->inst[0x6] = env16_lde;
    env16->inst[0x7] = env16_str;
    env16->inst[0x8] = env16_ldr;
    env16->inst[0x9] = env16_jmp;
    env16->inst[0xA] = env16_jpz;
    env16->inst[0xB] = env16_add;
    env16->inst[0xC] = env16_sub;
    env16->inst[0xD] = env16_div;
    env16->inst[0xE] = env16_mul;
    env16->inst[0xF] = env16_hlt;


    fseek(*exf, 0, SEEK_END);
    size_t fsize = ftell(*exf);
    fseek(*exf, 0, SEEK_SET);

    BYTE* exfbuff = malloc(fsize);
    fread(exfbuff, fsize, 1, *exf);

    memcpy(env16->ram, exfbuff, fsize);

    fclose(*exf);

    printf("Executable file size: %d Bytes\n", fsize);

    return EXIT_SUCCESS;
}

void*
handleWindow() {

    while (winrunning) {

        render();
        handleEvents();

    }

    return 0;
}

int
main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* exf;
    exf = fopen(argv[1], "r");

    if (exf == NULL) {
        printf("Couldn't open the file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    Env16Machine env16;
    pthread_t winthrd;

    initEnv16(&env16, &exf);

    dump_cpu = &env16.cpu;
    signal(SIGTSTP, dumpReg);
    signal(SIGINT, ctrlcHandler);

    initDisplay(ENV16_DPY_X, ENV16_DPY_Y, (void*) env16.ram + ENV16_VIDMEM_OFFST);

    pthread_create(&winthrd, NULL, handleWindow, NULL);

    while(env16.cpu.CYCLE++, running) {
        (env16.inst[(env16.cpu.CURRENT_OPCODE = HI_NIBBLE(env16.ram[env16.cpu.PC]))])(&env16);
    }

    pthread_join(winthrd, NULL);

    dumpReg();

    return 0;
}