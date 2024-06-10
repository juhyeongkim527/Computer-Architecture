/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

		/* TODO: */

        // register, pc 초기화
        state.pc = 0;
        for(int i = 0; i < NUMREGS; i++){
            state.reg[i] = 0;
        }

        int opcode;
        int regA;
        int regB;
        int offsetFields;
        int destReg;

        int instructions_count = 0;

        while(1){
            printState(&state);

            opcode = (state.mem[state.pc] >> 22) & 0b111;
            regA = (state.mem[state.pc] >> 19) & 0b111;
            regB = (state.mem[state.pc] >> 16) & 0b111;
            offsetFields = convertNum(state.mem[state.pc] & 0xFFFF);
            destReg = state.mem[state.pc] & 0b111;

            if(opcode == 0){ // add
                state.reg[destReg] = state.reg[regA] + state.reg[regB];
                state.pc++;
            }

            else if(opcode == 1){ // nor
                state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
                state.pc++;
            }

            else if(opcode == 2){ // lw
                state.reg[regB] = state.mem[state.reg[regA] + offsetFields];
                state.pc++;
            }

            else if(opcode == 3){ // sw
                state.mem[state.reg[regA] + offsetFields] = state.reg[regB];
                state.pc++;
            }

            else if(opcode == 4){ // beq
                if(state.reg[regA] == state.reg[regB]){
                    state.pc++;
                    state.pc += offsetFields;
                }
                else{
                    state.pc++;
                }
            }

            else if(opcode == 5){ // jalr
                state.reg[regB] = state.pc + 1;
                state.pc = state.reg[regA];
            }

            else if(opcode == 6){ // halt
                instructions_count++;
                printf("machine halted\n");
                printf("total of %d instructions executed\n", instructions_count);
                printf("final state of machine:\n");
                state.pc++;
                break;
            }

            else if(opcode == 7){ // noop
                state.pc++;
            }

            instructions_count++;
        }

    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
