███████ ███    ██ ██    ██         ██   ██████  
██      ████   ██ ██    ██       ████  ██       
█████   ██ ██  ██ ██    ██ █████   ██  ███████  
██      ██  ██ ██  ██  ██          ██  ██    ██ 
███████ ██   ████   ████          ████  ██████  

26/07/2022
VirtualException
@samu.efi

ENV-16

A 16-bit virtual environment made of an assembly-like language and a
minimalistic emulator

___________________________

ENV(-VM)-16 Specifications:
___________________________

-> 16-bit architecture
-> 65536 (2^16) Bytes of ramdom access memory (RAM)
-> 5 16-bit registers:
    rA (Primary, used for results and I/O)
    rB
    rC
    rD
    rE
-> Program Counter (PC)
-> Display: 128x72 Screen (Last 36864 Bytes of RAM)

________________

Instruction set:
________________

Notes:

In memory and jump instructions, the opcode contains an optional register which
can be used by placing a register name.

        (1st bit indicates the use of a register)
                           v
Example: str rB  -->> 0111 1[001].00000000.00000000
                              v
                           Register
// Stores the value of rA in the memory address stored in rB


0x0                  0x7000           0x10000
v                    v                v
.---------.----------.----------------.
| Program |   Free   |  Video Memory  |
'---------'----------'----------------'


                         Machine Code (Binary)
                        8 bits   8 bits   8 bits
                      .---^---. .--^^--. .--^^--.

Name & Desc.          Opcode    Arguments           Length
v                     v         v                   v

nop         	    : 0000 0000                     8 bits (1 byte)

....No operation

                            Index   Index
                                v   v
mov <reg1> <reg2>   : 0001 0000.00000000            16 bits (2 bytes)

....Switch <reg1> with <reg2>

                                <     Value     >
                                v               v
lda <ival>          : 0010 0000.00000000.00000000
ldb <ival>          : 0011 0000.00000000.00000000
ldc <ival>          : 0100 0000.00000000.00000000
ldd <ival>          : 0101 0000.00000000.00000000
lde <ival>          : 0110 0000.00000000.00000000   24 bits (3 bytes)

....Load the immediate <val> into rX

                           <rX> <    Address    >
                           v    v               v
str <addr/rX>       : 0111 0000.00000000.00000000   24 bits (3 bytes)

....Store the value of rA into memory address <addr>

                           <rX> <    Address    >
                           v    v               v
ldr <addr/rX>       : 1000 0000.00000000.00000000   24 bits (3 bytes)

....Store the value at memory address <addr> into rA

                           <rX> <     Value     >
                           v    v               v
jmp <val/rX>        : 1001 0000.00000000.00000000   24 bits (3 bytes)

....Set the PC to <val> (or rX)

                           <rX> <     Value     >
                           v    v               v
jpz <val/rX>        : 1010 0000.00000000.00000000   24 bits (3 bytes)

....Set the PC to <val> (or rX) if rA == 0

                            Index   Index
                                v   v
add <reg1> <reg2>   : 1011 0000.00000000            16 bits (2 bytes)

....Add reg1 with reg2 and store the result on rA

                            Index   Index
                                v   v
sub <reg1> <reg2>   : 1100 0000.00000000            16 bits (2 bytes)

....Subtract reg1 from reg2 and store the result on rA

                            Index   Index
                                v   v
div <reg1> <reg2>   : 1101 0000.00000000            16 bits (2 bytes)

....Divide reg1 by reg2 and store the result on rA

                            Index   Index
                                v   v
mul <reg1> <reg2>   : 1110 0000.00000000            16 bits (2 bytes)

....Multiply reg1 by reg2 and store the result on rA

hlt                 : 1111 0000                     8 bits (1 byte)

....Halt the cpu