||||||| |||    || ||    ||         ||   ||||||
||      ||||   || ||    ||       ||||  ||
|||||   || ||  || ||    || |||||   ||  |||||||
||      ||  || ||  ||  ||          ||  ||    ||
||||||| ||   ||||   ||||          ||||  ||||||

26/07/2022
VirtualException
@samu.efi

ENV-16

A 16-bit virtual environment made of an assembly-like language and a
minimalistic emulator

___________________________

ENV-16 Specifications:
___________________________

-> 16-bit architecture
-> 65536 (2^16) Bytes of ramdom access memory (RAM)
-> 5 16-bit registers:
    rA (Primary, used for results and I/O)
    rB, rC, rD, rE
-> 24-bit Instructions
-> Program Counter (PC)
-> Display: 128x72 Screen (Last 28672 Bytes of RAM)

0x0                          0x7000         0x10000
v                            v              v
.---------.------------------.--------------.
| Program |       Free       | Video Memory |
'---------'------------------'--------------'

___________________________

Instruction set & encoding:
___________________________

There are 16 different instructions, 4 of them with 5 variants; one for
each register plus the one with the inmmediate value. This instruction are
marked with a '*'.

Notes:

In memory and jump instructions, the opcode contains an optional register which
can be used by placing a register name.

        (1st bit indicates the use of a register)
                           v
Example: str rB  -->> 0111 1[001].00000000.00000000
                              v
                           Register
// Stores the value of rA in the memory address stored in rB


                    (Machine Code, 24-bit enconding)
                        8 bits       16 bits
                      .---^---. .-------^-------.

Name & Desc.          Opcode    Arguments
|                     |         |
v                     v         v

nop         	    : 0000 0000.00000000.00000000

    No operation

                                <Index > < Index>
                                v      v v      v
mov <reg1> <reg2>   : 0001 0000.00000000.00000000

    Move <reg1> into <reg2>

                                <     Value     >
                                v               v
lda <ival>          : 0010 0000.00000000.00000000
ldb <ival>          : 0011 0000.00000000.00000000
ldc <ival>          : 0100 0000.00000000.00000000
ldd <ival>          : 0101 0000.00000000.00000000
lde <ival>          : 0110 0000.00000000.00000000

    Load the immediate <val> into rX

                           <rX> <    Address    >
                           v  v v               v
str <addr/rX>       : 0111 0000.00000000.00000000

    Store the value of rA into memory address <addr>    *

                           <rX> <    Address    >
                           v  v v               v
ldr <addr/rX>       : 1000 0000.00000000.00000000

    Store the value at memory address <addr> into rA    *

                           <rX> <     Value     >
                           v  v v               v
jmp <val/rX>        : 1001 0000.00000000.00000000

    Set the PC to <val>                                 *

                           <rX> <     Value     >
                           v  v v               v
jpz <val/rX>        : 1010 0000.00000000.00000000

    Set the PC to <val> if rA is 0                      *

                                <Index > < Index>
                                v      v v      v
add <reg1> <reg2>   : 1011 0000.00000000.00000000

    Add reg1 with reg2 and store the result on rA

                                <Index > < Index>
                                v      v v      v
sub <reg1> <reg2>   : 1100 0000.00000000.00000000

    Subtract reg1 from reg2 and store the result on rA

                                <Index > < Index>
                                v      v v      v
div <reg1> <reg2>   : 1101 0000.00000000.00000000

    Divide reg1 by reg2 and store the result on rA

                                <Index > < Index>
                                v      v v      v
mul <reg1> <reg2>   : 1110 0000.00000000.00000000

    Multiply reg1 by reg2 and store the result on rA

hlt                 : 1111 0000.00000000.00000000

    Halt the cpu