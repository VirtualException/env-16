#ifndef _ENV16_H
#define _ENV16_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>

#define _ENV16DBG 1

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

#define CURR_BYTE(a)    ((a)->ram[(a)->cpu.PC])

#define ISBITSET(byte, n) ((byte & (128 >> n)) == 1)

#define ENV16_MEMBYTES      0x10000

#define ENV16_VIDMEM_SIZE   0x4800
#define ENV16_VIDMEM_OFFST  0xB800

#define ENV16_DPY_X         128
#define ENV16_DPY_Y         72

#define ENV16_INST          16


enum opcodes {
    NOP = 0b0000,
    SWT = 0b0001,
    LDA = 0b0010,
    LDB = 0b0011,
    LDC = 0b0100,
    LDD = 0b0101,
    LDE = 0b0110,
    STR = 0b0111,
    LDR = 0b1000,
    JMP = 0b1001,
    JPZ = 0b1010,
    ADD = 0b1011,
    SUB = 0b1100,
    DIV = 0b1101,
    MUL = 0b1110,
    HLT = 0b1111,
};

typedef uint8_t     BYTE;
typedef uint16_t    WORD;
typedef uint32_t    DWORD;
typedef uint64_t    QWORD;

struct _Env16Machine;

typedef
WORD
(*Env16Func) (
    struct _Env16Machine* env16
);

typedef struct {

    WORD PC;

    WORD rA;
    WORD rB;
    WORD rC;
    WORD rD;
    WORD rE;

    QWORD CYCLE;

    BYTE _DBG_CURRENT_OPCODE;

#define rSTART rA

} Env16Cpu;

typedef struct {

    void* v;

} Env16Dpy;

typedef struct _Env16Machine  {

    Env16Cpu    cpu;
    BYTE*       ram;

    Env16Func   inst[ENV16_INST];

    Env16Dpy    dpy;

} Env16Machine;

void    ctrlcHandler(int i);
void    dumpReg();
int     initEnv16(Env16Machine* env16, FILE** exf);
void*   handleWindow();

extern bool running;
extern bool winrunning;

#endif