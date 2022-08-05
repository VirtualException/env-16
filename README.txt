 ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄               ▄         ▄▄▄▄      ▄▄▄▄▄▄▄▄▄▄▄
▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░▌             ▐░▌      ▄█░░░░▌    ▐░░░░░░░░░░░▌
▐░█▀▀▀▀▀▀▀▀▀ ▐░▌░▌     ▐░▌ ▐░▌           ▐░▌      ▐░░▌▐░░▌    ▐░█▀▀▀▀▀▀▀▀▀
▐░▌          ▐░▌▐░▌    ▐░▌  ▐░▌         ▐░▌        ▀▀ ▐░░▌    ▐░▌
▐░█▄▄▄▄▄▄▄▄▄ ▐░▌ ▐░▌   ▐░▌   ▐░▌       ▐░▌▄▄▄▄▄▄▄▄▄▄▄ ▐░░▌    ▐░█▄▄▄▄▄▄▄▄▄
▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌    ▐░▌     ▐░▌▐░░░░░░░░░░░▌▐░░▌    ▐░░░░░░░░░░░▌
▐░█▀▀▀▀▀▀▀▀▀ ▐░▌   ▐░▌ ▐░▌     ▐░▌   ▐░▌  ▀▀▀▀▀▀▀▀▀▀▀ ▐░░▌    ▐░█▀▀▀▀▀▀▀█░▌
▐░▌          ▐░▌    ▐░▌▐░▌      ▐░▌ ▐░▌               ▐░░▌    ▐░▌       ▐░▌
▐░█▄▄▄▄▄▄▄▄▄ ▐░▌     ▐░▐░▌       ▐░▐░▌            ▄▄▄▄█░░█▄▄▄ ▐░█▄▄▄▄▄▄▄█░▌
▐░░░░░░░░░░░▌▐░▌      ▐░░▌        ▐░▌            ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌
 ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀          ▀              ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀

26/07/2022
VirtualException
@samu.efi

ENV-16

A 16-bit virtual environment made of an assembly-like language and a minimalistic emulator

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
-> Display: 128x72 Screen (Last 18432 Bytes of RAM)

________________

Instruction set:
________________

Notes:

In memory and jump instructions, the opcode contains an optional register which can
be used by placing a register name.

        (1st bit indicates the use of a register)
                           v
Example: str rB  -->> 0111 1[001].00000000.00000000     // Stores the value of rA in the memory address stored in rB
                              v
                           Register

0x0                                    0xB800         0x10000
v                                      v              v
.-------------------.------------------.--------------.
|      Program      |       Free       | Video Memory |
'-------------------'------------------'--------------'


                         Machine Code (Binary)
                        8 bits   8 bits   8 bits
                      .---^---. .--^^--. .--^^--.

Name                  Opcode    Arguments           Description                                             Length
v                     v         v                   v                                                       v

nop         	    : 0000 0000                     => No operation                                         [8-bits] (1 byte)

                            Index   Index
                                v   v
swt <reg1> <reg2>   : 0001 0000.00000000            => Switch <reg1> with <reg2>                            [16-bits] (2 bytes)

                                <     Value     >
                                v               v
lda <ival>          : 0010 0000.00000000.00000000
ldb <ival>          : 0011 0000.00000000.00000000
ldc <ival>          : 0100 0000.00000000.00000000
ldd <ival>          : 0101 0000.00000000.00000000
lde <ival>          : 0110 0000.00000000.00000000   => Load the immediate <val> into rX                     [24-bits] (3 bytes)

                           <rX> <    Address    >
                           v    v               v
str <addr/rX>       : 0111 0000.00000000.00000000   => Store the value of rA into memory address <addr>     [24-bits] (3 bytes)
ldr <addr/rX>       : 1000 0000.00000000.00000000   => Store the value at memory address <addr> into rA     [24-bits] (3 bytes)

                           <rX> <     Value     >
                           v    v               v
jmp <val/rX>        : 1001 0000.00000000.00000000   => Set the PC to <val> (or the value of rX)             [24-bits] (3 bytes)
jpz <val/rX>        : 1010 0000.00000000.00000000   => Set the PC to <val> (or the value of rX) if rA == 0  [24-bits] (3 bytes)

                            Index   Index
                                v   v
add <reg1> <reg2>   : 1011 0000.00000000            => Add reg1 with reg2 and store the result on rA        [16-bits] (2 bytes)
sub <reg1> <reg2>   : 1100 0000.00000000            => Subtract reg1 from reg2 and store the result on rA   [16-bits] (2 bytes)
div <reg1> <reg2>   : 1101 0000.00000000            => Divide reg1 by reg2 and store the result on rA       [16-bits] (2 bytes)
mul <reg1> <reg2>   : 1110 0000.00000000            => Multiply reg1 by reg2 and store the result on rA     [16-bits] (2 bytes)

hlt                 : 1111 0000                     => Halt the cpu                                         [8-bits] (1 byte)
