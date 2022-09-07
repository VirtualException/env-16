#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <memory.h>

#define BUILDNUM "2.1"

#define INVALID_ARGUMENT    0
#define EXPECTED_ARGUMENT   1
#define VALUE_OVERFLOW      2
#define NOT_SUPPORTED       3
#define EXTRA_ARGUMENT      4
#define INVALID_LABEL       5

struct label {
    char name[16];
    uint16_t addr;
};

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

char* opcode_txt[] = {
    "nop",
    "mov",
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

struct label labels[32] = {0};
size_t max_label = 0;


int writeInstruction(uint8_t opcode, uint16_t args, FILE** fd) {

    fwrite(&opcode, 1, 1, *fd);
    fwrite(&args, 1, 2, *fd);

    return EXIT_SUCCESS;
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

    case INVALID_LABEL:
        printf("Error at line %d, INVALID_LABEL\n", line);
        break;

    default:
        printf("Error at line %d", line);
        break;
    }

    fclose(inf);
    fclose(outf);

    exit(EXIT_FAILURE);

}

int reg2num(char* kwrd, uint8_t* regN) {

    if (kwrd[0] == 'r' && kwrd[1] >= 'A' && kwrd[1] <= 'E' && kwrd[2] == '\0') {
       *regN = kwrd[1] - 65;
       return 0;
    }
    else return 1;

}

int getLabel(char* label) {

    if(label == NULL) return -2;

    for (size_t i = 0; i < max_label ; i++) {
        if (!strcmp(label, labels[i].name)) return labels[i].addr;
    }

    return -1;

}

int getOpcode(char* kwrd) {

    for (size_t i = 0; i < 16; i++) {
        if (!memcmp(kwrd, opcode_txt[i], 3)) {
            return i;
        }
    }
    return -1;

}

int
main(int argc, char* argv[]) {

    // Argument checking

    if (argc != 3) {
        if (argc == 2 && !strcmp(argv[1], "-v")) {

            printf("ENV16 Compiler for x86_64\nBuild: %s (%s)\n", BUILDNUM, __TIMESTAMP__);
            printf("Usage: %s [-v] <input> <output>\n", argv[0]);

            exit(EXIT_SUCCESS);
        }

        fprintf(stderr, "Usage: %s [-v] <input> <output>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((inf = fopen(argv[1], "r")) == 0) {
        fprintf(stderr, "File \"%s\" does not exist\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    outf = fopen(argv[2], "w");

    // File-related variables

    size_t currline = 0;
    size_t currbyte = 0;

    size_t currlabel = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int opcode = 0;

    int i = 1;

    // Preprocess file

    while((read = getline(&line, &len, inf)) != EOF) {

        char*   pch         = NULL;
        size_t  pch_len     = 0;
        char    label[16];

        currline++;

        if(strlen(line) <= 2) goto skip1;
        else if (line[read-1] == '\n') line[read-1] = 0; // Remove the new line character

        pch = strtok(line, " ");

        if (pch[0] == ';') {
            goto skip1;
        }

        if ((opcode = getOpcode(pch)) >= 0) {
            currbyte += 3;
            goto skip1;
        }

        pch_len = strlen(pch);

        if (pch[pch_len-1] != ':') {
            goto skip1;
        }

        if ((pch_len = strlen(pch)) > 16 || strtok(NULL, " ") != NULL) {
            error(currline, INVALID_LABEL);
        }

        strcpy(labels[currlabel].name, pch);
        labels[currlabel].addr = currbyte;
        currlabel++;

skip1:
    }

    max_label = currlabel;

    printf("\n[DEBUG] Labels:\n");

    for (size_t i = 0; i < max_label; i++) {
        printf("%s \t-> \t0x%X\n", labels[i].name, labels[i].addr);
    }

    printf("\n");

    fseek(inf, 0, SEEK_SET);
    currline = 0;

    // Process instructions

    while((read = getline(&line, &len, inf)) != EOF) {

        char*       pch     = NULL;
        uint8_t     reg1    = 0;
        uint8_t     reg2    = 0;
        long int    num     = 0;
        size_t      addr    = 0;

        currline++;

        if(strlen(line) <= 2) goto skip2;
        else if (line[read-1] == '\n') line[read-1] = 0; // Remove the new line character

        printf("[DEBUG] Analizing line Nº %d: \"%s\"\n", currline, line);

        pch = strtok(line, " ");

        if (pch[0] == ';') {
            goto skip2;
        }

        if((opcode = getOpcode(pch)) < 0) {
            if (getLabel(pch) < 0) {
                error(currline, NOT_SUPPORTED);
            }
            else goto skip2;
        }

        switch ((uint8_t) opcode) {

        case NOP:
        case HLT:

            // Write instruction
            writeInstruction(opcode << 4, 0, &outf);

            break;

        case MOV:

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

            // Write instruction
            writeInstruction(opcode << 4, ((uint16_t) reg2 << 8) | reg1, &outf);

            break;

        case LDA:
        case LDB:
        case LDC:
        case LDD:
        case LDE:

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, EXPECTED_ARGUMENT);
            }

            num = strtol(pch, NULL, 0);
            if (num > 65535) {
                error(currline, VALUE_OVERFLOW);
            }

            // Write instruction
            writeInstruction(opcode << 4, (uint16_t) num, &outf);

            break;

        case STR:
        case LDR:

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, EXPECTED_ARGUMENT);
            }

            if (reg2num(pch, &reg1)) {

                num = strtol(pch, NULL, 0);
                if (num > 65535) {
                    error(currline, VALUE_OVERFLOW);
                }

                // Write instruction
                writeInstruction(opcode << 4, (uint16_t) num, &outf);

            }
            else { 

                // Write instruction (with register)
                writeInstruction((opcode << 4 | reg1) | 0b0001000, 0, &outf);

            }

            break;

        case JMP:
        case JPZ:

            // Next keyword
            if ((pch = strtok(NULL, " ")) == NULL) {
                error(currline, EXPECTED_ARGUMENT);
            }

            if (!reg2num(pch, &reg1)) {

                // Write instruction (with register)
                writeInstruction((opcode << 4 | reg1) | 0b0001000, 0, &outf);

            }
            else if ((addr = getLabel(pch)) >= 0) {

                // Write instruction (with label address)
                writeInstruction(opcode << 4, (uint16_t) addr, &outf);

            }
            else { 

                num = strtol(pch, NULL, 0);
                if (num > 65535) {
                    error(currline, VALUE_OVERFLOW);
                }

                // Write instruction (with immediate)
                writeInstruction(opcode << 4, (uint16_t) num, &outf);

            }

            break;

        case ADD:
        case SUB:
        case DIV:
        case MUL:

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

            // Write instruction
            //printf("[%d, %d]", reg1, reg2);
            writeInstruction(opcode << 4, ((uint16_t) reg1 << 8) | reg2, &outf);

            currbyte += 3;

            break;

        default:

            error(currline, NOT_SUPPORTED);

            break;
        }

        // Check for extra argument in the line that is not a comment
        if ((pch = strtok(NULL, " ")) != NULL && pch[0] != ';') {
            error(currline, EXTRA_ARGUMENT);
        }
skip2:
    }

    fclose(inf);
    fclose(outf);

    return 0;
}