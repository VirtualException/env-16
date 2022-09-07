#ifndef _ENV16_H
#define _ENV16_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>

#include "config.h"

#define ENV16_MEMBYTES      0x10000

#define ENV16_VIDMEM_SIZE   (ENV16_DPY_X * ENV16_DPY_Y * ENV16_BPP)
#define ENV16_VIDMEM_OFFST  (ENV16_MEMBYTES - ENV16_VIDMEM_SIZE)

#define ENV16_INST_BYTE_LEN 3

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

#define ENV16_INST 16


enum opcodes {
    NOP,
    MOV,
    LDA,
    LDB,
    LDC,
    LDD,
    LDE,
    STR,
    LDR,
    JMP,
    JPZ,
    ADD,
    SUB,
    DIV,
    MUL,
    HLT,
};

typedef uint8_t     BYTE;
typedef uint16_t    WORD;
typedef uint32_t    DWORD;
typedef uint64_t    QWORD;

struct _Env16Machine;

typedef
void
(*Env16Func) (
    struct _Env16Machine* env16
);

typedef struct {

    volatile WORD PC;

    volatile WORD rA;
    volatile WORD rB;
    volatile WORD rC;
    volatile WORD rD;
    volatile WORD rE;

    volatile QWORD CYCLE;

    volatile BYTE CURRENT_OPCODE;

#define rSTART rA

} Env16Cpu;

typedef struct _Env16Machine  {

    Env16Cpu    cpu;
    BYTE*       ram;

    Env16Func   inst[ENV16_INST];

} Env16Machine;


void    ctrlcHandler(int i);
void    dumpReg();
int     initEnv16(Env16Machine* env16, FILE** exf);
void*   handleWindow();

extern bool running;
extern bool winrunning;

#endif