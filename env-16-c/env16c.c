#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <memory.h>

#define INVALID_ARGUMENT    0
#define EXPECTED_ARGUMENT   1
#define VALUE_OVERFLOW      2
#define NOT_SUPPORTED       3
#define EXTRA_ARGUMENT      4

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

char* OpcodesText[] = {
    "nop",
    "swt",
    "lda",
    "ldb",
    "ldc",
    "ldd",
    "lde",
    "str",
    "ldr",
    "jmp",
    "jpz",
    "add",
    "sub",
    "div",
    "mul",
    "hlt",
};

FILE* inf;
FILE* outf;

static inline int writeByte(uint8_t byte, FILE** fd) {

    fwrite(&byte, 1, 1, *fd);
    return EXIT_SUCCESS;

}

int reg2num(char* kwrd, uint8_t* regN) {

    if (kwrd[0] == 'r' && kwrd[1] >= 'A' && kwrd[1] <= 'E') {
       *regN = kwrd[1] - 65;
       return 0;
    }
    else return 1;

}

int error(int line, int cause) {

    switch (cause) {
    case INVALID_ARGUMENT:
        printf("Error at line %d, INVALID_ARGUMENT\n", line);
        break;

    case EXPECTED_ARGUMENT:
        printf("Error at line %d, EXPECTED_ARGUMENT\n", line);
        break;

    case VALUE_OVERFLOW:
        printf("Error at line %d, VALUE_OVERFLOW\n", line);
        break;

    case NOT_SUPPORTED:
        printf("Error at line %d, NOT_SUPPORTED\n", line);
        break;

    case EXTRA_ARGUMENT:
        printf("Error at line %d, EXTRA_ARGUMENT\n", line);
        break;

    default:
        printf("Error at line %d", line);
        break;
    }

    fclose(inf);
    fclose(outf);

    exit(EXIT_FAILURE);

}

int getOpcode(char* kwrd) {

    for (size_t i = 0; i < 16; i++) {
        if (!memcmp(kwrd, OpcodesText[i], 3)) {
            return i;
        }
    }
    return -1;

}

int
main(int argc, char* argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((inf = fopen(argv[1], "r")) == 0) {
        fprintf(stderr, "File \"%s\" does not exist\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    outf = fopen(argv[2], "w");

    size_t currline = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int opcode = 0;

    while((read = getline(&line, &len, inf)) != EOF) {

        currline++;

        char*       pch     = NULL;
        uint8_t     reg1    = 0;
        uint8_t     reg2    = 0;
        long int    num     = 0;
        uint16_t    num16   = 0;

        if (line[read-1] == '\n') line[read-1] = 0; // Remove the new line character

        printf("[DEBUG] Analizing line Nº %d: \"%s\"\n", currline, line);

        if(!strlen(line)) goto skip;

        pch = strtok(line, " ");

        if((opcode = getOpcode(pch)) < 0) {
            error(currline, NOT_SUPPORTED);
        }

        switch (opcode) {

        case NOP:
        case HLT:

            writeByte(opcode << 4, &outf);

            break;

        case SWT:

            // Write opcode to file
            writeByte(opcode << 4, &outf);

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, EXPECTED_ARGUMENT);
            }

            if (reg2num(pch, &reg1)) {
                error(currline, INVALID_ARGUMENT);
            }

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, INVALID_ARGUMENT);
            }

            if (reg2num(pch, &reg2)) {
                error(currline, INVALID_ARGUMENT);
            }

            // Write arguments to the file
            writeByte(reg1 << 4 | reg2, &outf);

            break;

        case LDA:
        case LDB:
        case LDC:
        case LDD:
        case LDE:

            // Write opcode
            writeByte(opcode << 4, &outf);

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, EXPECTED_ARGUMENT);
            }

            num = strtol(pch, NULL, 0);
            if (num > 65535) {
                error(currline, VALUE_OVERFLOW);
            }

            // Write 16-bit number
            num16 = (uint16_t) num;
            fwrite(&num16, sizeof(uint16_t), 1, outf);

            break;

        case STR:
        case LDR:

        case JMP:
        case JPZ:

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, EXPECTED_ARGUMENT);
            }

            if (reg2num(pch, &reg1)) {

                num = strtol(pch, NULL, 0);
                if (num > 65535) {
                    error(currline, VALUE_OVERFLOW);
                }

                // Write opcode
                writeByte(opcode << 4, &outf);

                // Write 16-bit number
                num16 = (uint16_t) num;
                fwrite(&num16, sizeof(uint16_t), 1, outf);

            }
            else { 
                // Write opcode with register
                writeByte(opcode << 4 | reg1 | 0b0001000, &outf);

                // Write 16-bits empty
                writeByte(0, &outf);
                writeByte(0, &outf);
            }

            break;

        case ADD:
        case SUB:
        case DIV:
        case MUL:

            // Write opcode
            writeByte(opcode << 4, &outf);

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, EXPECTED_ARGUMENT);
            }

            uint8_t reg1;
            uint8_t reg2;

            if (reg2num(pch, &reg1)) {
                error(currline, INVALID_ARGUMENT);
            }

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, INVALID_ARGUMENT);
            }

            if (reg2num(pch, &reg2)) {
                error(currline, INVALID_ARGUMENT);
            }

            // Write arguments to the file
            writeByte(reg1 << 4 | reg2, &outf);

            break;

        default:

            error(currline, NOT_SUPPORTED);

            break;
        }

        // Check for extra argument in the line that is not a comment
        if ((pch = strtok(NULL, " ")) != NULL && pch[0] != ';') {
            error(currline, EXTRA_ARGUMENT);
        }
skip:
    }

    fclose(inf);
    fclose(outf);

    return 0;
}