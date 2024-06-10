/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define NUMMEMORY 65536 /* maximum number of words in memory */

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* TODO: Phase-1 label calculation */
	typedef struct {
		char name[7];
		int address;
	} label_struct;

	label_struct labels[NUMMEMORY];
	int label_count = 0;
	int line_number = 0;

	while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
		if(strcmp(label, "")){

			if(strlen(label) > 6 || isNumber(label)){
				printf("Error - Label should be represented by up to 6 characters : %s\n", label);
				exit(1);
			}
			int dup_check = 0;
			strcpy(labels[label_count].name, label);
			labels[label_count].address = line_number;
			label_count++;

			for(int i = 0; i < label_count; i++){
				if(!strcmp(labels[i].name, label))
				dup_check++;
			}

			if(dup_check > 1){
				printf("Error - Duplicated definition of label : %s\n", label);
				exit(1);
			}
		}
		line_number++;
	}

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	// inFilePtr를 파일 시작점으로 이동
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */

	/* after doing a readAndParse, you may want to do the following to test the
		 opcode */

	line_number = 0;
	while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
		int machine_code = 0;

		if (!strcmp(opcode, "add")){
			if(!isNumber(arg0) || !isNumber(arg1) || !isNumber(arg2)){
				printf("Error - Non-integer register names\n");
				exit(1);
			}
			if(atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7 || atoi(arg2) < 0 || atoi(arg2) > 7){
				printf("Error - Registers out of range\n");
				exit(1);
			}
			machine_code += atoi(arg0)<<19;
			machine_code += atoi(arg1)<<16; 
			machine_code += atoi(arg2);
		}

		else if (!strcmp(opcode, "nor")) {
			if(!isNumber(arg0) || !isNumber(arg1) || !isNumber(arg2)){
				printf("Error - Non-integer register names\n");
				exit(1);
			}
			if(atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7 || atoi(arg2) < 0 || atoi(arg2) > 7){
				printf("Error - Registers out of range\n");
				exit(1);
			}
			machine_code += 1<<22;
			machine_code += atoi(arg0)<<19;
			machine_code += atoi(arg1)<<16; 
			machine_code += atoi(arg2);
		}

		else if (!strcmp(opcode, "lw")) {
			if(!isNumber(arg0) || !isNumber(arg1)){
				printf("Error - Non-integer register names\n");
				exit(1);
			}
			if(atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7){
				printf("Error - Registers out of range\n");
				exit(1);
			}
			machine_code += 2<<22;
			machine_code += atoi(arg0)<<19;
			machine_code += atoi(arg1)<<16;
			if(isNumber(arg2)){
				if(atoi(arg2) < -32768 || atoi(arg2) > 32767){
					printf("Error - offsetFields that does not fit in 16 bits : %s\n", arg2);
					exit(1);
				}
				machine_code += atoi(arg2) & 0xFFFF; // 음수인 경우 2's 상위 16비트 날리기 위해
			}
			else{
				int is_label_found = 0;
				for(int i = 0; i < label_count; i++){
					if(!strcmp(labels[i].name, arg2)){
						if(labels[i].address < -32768 || labels[i].address > 32767){
							printf("Error - offsetFields that does not fit in 16 bits : %s\n", arg2);
							exit(1);
						}
						machine_code += labels[i].address & 0xFFFF; // 음수인 경우 2's 상위 16비트 날리기 위해
						is_label_found++;
						break;
					} 
				}
				if(!is_label_found){
					printf("Error - Use of undefined labels : %s\n", arg2);
					exit(1);
				}
			}
		}
		else if (!strcmp(opcode, "sw")) {
			if(!isNumber(arg0) || !isNumber(arg1)){
				printf("Error - Non-integer register names\n");
				exit(1);
			}
			if(atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7){
				printf("Error - Registers out of range\n");
				exit(1);
			}
			machine_code += 3<<22;
			machine_code += atoi(arg0)<<19;
			machine_code += atoi(arg1)<<16;
			if(isNumber(arg2)){
				if(atoi(arg2) < -32768 || atoi(arg2) > 32767){
					printf("Error - offsetFields that does not fit in 16 bits : %s\n", arg2);
					exit(1);
				}
				machine_code += atoi(arg2) & 0xFFFF; // 음수인 경우 2's 상위 16비트 날리기 위해
			}
			else{
				int is_label_found = 0;
				for(int i = 0; i < label_count; i++){
					if(!strcmp(labels[i].name, arg2)){
						if(labels[i].address < -32768 || labels[i].address > 32767){
							printf("Error - offsetFields that does not fit in 16 bits : %s\n", arg2);
							exit(1);
						}
						machine_code += labels[i].address & 0xFFFF; // 음수인 경우 2's 상위 16비트 날리기 위해
						is_label_found++;
						break;
					} 
				}
				if(!is_label_found){
					printf("Error - Use of undefined labels : %s\n", arg2);
					exit(1);
				}
			}
		}

		else if (!strcmp(opcode, "beq")) {
			if(!isNumber(arg0) || !isNumber(arg1)){
				printf("Error - Non-integer register names\n");
				exit(1);
			}
			if(atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7){
				printf("Error - Registers out of range\n");
				exit(1);
			}
			machine_code += 4<<22;
			machine_code += atoi(arg0)<<19;
			machine_code += atoi(arg1)<<16;
			if(isNumber(arg2)){
				if(atoi(arg2) < -32768 || atoi(arg2) > 32767){
					printf("Error - offsetFields that does not fit in 16 bits : %s\n", arg2);
					exit(1);
				}
				machine_code += atoi(arg2) & 0xFFFF; // 음수인 경우 2's 상위 16비트 날리기 위해
			}
			else{
				int is_label_found = 0;
				for(int i = 0; i < label_count; i++){
					if(!strcmp(labels[i].name, arg2)){
						if(labels[i].address - line_number - 1 < -32768 || labels[i].address - line_number - 1 > 32767){
							printf("Error - offsetFields that does not fit in 16 bits : %s\n", arg2);
							exit(1);
						}
						machine_code += (labels[i].address - line_number - 1) & 0xFFFF; // 음수인 경우 2's 상위 16비트 날리기 위해
						is_label_found++;
						break;
					} 
				}
				if(!is_label_found){
					printf("Error - Use of undefined labels : %s\n", arg2);
					exit(1);
				}
			}
		}

		else if (!strcmp(opcode, "jalr")) {
			if(!isNumber(arg0) || !isNumber(arg1)){
				printf("Error - Non-integer register names\n");
				exit(1);
			}
			if(atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7){
				printf("Error - Registers out of range\n");
				exit(1);
			}
			machine_code += 5<<22;
			machine_code += atoi(arg0)<<19;
			machine_code += atoi(arg1)<<16;
		}

		else if (!strcmp(opcode, "halt")) {
			machine_code += 6<<22;
		}

		else if (!strcmp(opcode, "noop")) {
			machine_code += 7<<22;
		}

		else if (!strcmp(opcode, ".fill")) {
			if(isNumber(arg0)){
				long int range_check = strtol(arg0, NULL, 10);
				if(range_check < -2147483648 || range_check > 2147483647) {
					printf("Error - The bounds of the numeric value for .fill instructions are -2147483648 to 2147483647 : %s\n", arg0);
					exit(1);
				}
				machine_code += atoi(arg0);
			}
			else{
				int is_label_found = 0;
				for(int i = 0; i < label_count; i++){
					if(!strcmp(labels[i].name, arg0)){
						machine_code += labels[i].address;
						is_label_found++;
						break;
					} 
				}
				if(!is_label_found){
					printf("Error - Use of undefined labels : %s\n", arg0);
					exit(1);
				}
			}
		}

		else{
			printf("Error - Unrecognized opcodes : %s\n", opcode);
			exit(1);
		}
		
		line_number++;
		fprintf(outFilePtr, "%d\n", machine_code);
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */

// 1줄의 라인을 읽어서 label, opcode, arg0, arg1, ar
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		// char *fgets (char *string, int n, FILE *stream);
		// stream에서 n개만큼의 문자열을 읽어들였으면('\n'이 오면 n개를 다 못읽어도 '\n'까지만(포함해서) 입력 받음) str에 저장하고 str을 가리키는 포인터를 리턴 
		// 오류나 EOF를 만났는데 아무런 string을 읽어들이지 못한 경우(내용이 없는 빈 파일인 경우) NULL 리턴
		
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		// char *strchr(const char *s, int c);
		// s에서 '\n'을 찾지 못한 경우 NULL을 리턴(한줄짜리 파일), 찾으면 '\n' 뒤의 문자를 가리키는 포인터 리턴

		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		// int sscanf(const char *buffer, const char *format, argument-list);
		// buffer에서 파일 포맷 형식을 읽어들인 후 argument-list(파일 포맷에 따라 변할 것임)에 저장
		// sscanf로 성공적으로 받아들인 값의 개수를 반환
		
		// %[X]   : X를 만나는 경우 X만 읽어들임
		// %*[X]  : X를 제외하고 읽어들임
		// %[^X]  : X직전까지의 값을 읽어들임
		// %*[^X] : X직전까지의 값을 버리고 X부터 읽음 (포인터는 X에 위치)
		
		// "%[^\t\n\r ]" 는 \t, \n, \r 을 만나기 전까지 ptr에서 읽어들인 후 label에 저장 

		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

