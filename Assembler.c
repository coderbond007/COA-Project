/*
This program uses Trie Data Structure to store the opcodes and their corresponding Binary Codes.
Set of Instructions in the file "instructions.txt" are converted to Binary Codes in "machineCodes.txt".
Details of each function is defined after it in comments.
*/

// C Standard Libraries used in the program is imported here.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


// NODE struct is used for TRIE Data Structure.
typedef struct NODE
{
	struct NODE* child[26];
	int leaf;
	char binarycode[16];
}node;

// All Functions are decleared here.
node* getnode();
node* insertKeyword(node* root, char* keyword, char* binaryCodeOfKeyword);
int convertDecimalToBinary(int number, char* binaryRepresentationOfDecimal);
int getBinaryCodeOfKeyword(node* root, char* keyword, char* machineCode);
int appendBinaryCodeOfOperand(char* machineCode, int operand_value, int start_from);


int main(int argc, char const *argv[])
{
	/* code */

	//root node of TRIE
	node* root = NULL;
	FILE* fp1;
	fp1 = fopen("opcode.txt","r");
	char opcode[100];
	char binarycode[100];

	// Scans the opcodes with their corresponding binary code from "opcode.txt"
	// and insert them into TRIE 
	while(fscanf(fp1,"%s %s",opcode,binarycode)!=EOF) {
		root = insertKeyword(root, opcode, binarycode);
	}	
	FILE* fp2;
	FILE* fp3;
	fp2 = fopen("instructions.txt","r");
	fp3 = fopen("machineCodes.txt","w");
	char current_instruction[100];
	int operand1;
	int operand2;
	int operand3;

	// Scans the instructions from the file "instructions.txt" and convert them into
	// binary codes. Binary Codes are written in the file "machinecodes.txt"
	while(fscanf(fp2,"%s",current_instruction) != EOF) {
		char machineCode[50];
		getBinaryCodeOfKeyword(root, current_instruction, machineCode);
		int lenOfInstruction = strlen(machineCode);

		// Validity of input is checked using this variable which will abort the program if
		// invalid input is provided
		int validity = 1;
		switch(lenOfInstruction) {
			case (4) :
				fscanf(fp2,"%d %d %d",&operand1,&operand2,&operand3);
				validity = validity * getBinaryCodeOfKeyword(root, current_instruction, machineCode);
				validity = validity * appendBinaryCodeOfOperand(machineCode,operand1,4);
				validity = validity * appendBinaryCodeOfOperand(machineCode,operand2,8);
				validity = validity * appendBinaryCodeOfOperand(machineCode,operand1,12);
				if(validity == -1){
					printf("Invalid Arguments\n");
					printf("Program Aborted\n");
					return 0;
				}
				fprintf(fp3, "%s\n", machineCode);
				break;
			case (8) :
				fscanf(fp2,"%d %d",&operand1,&operand2);
				validity = validity * getBinaryCodeOfKeyword(root, current_instruction, machineCode);
				validity = validity * appendBinaryCodeOfOperand(machineCode,operand1,8);
				validity = validity * appendBinaryCodeOfOperand(machineCode,operand2,12);
				if(validity == -1){
					printf("Invalid Arguments\n");
					printf("Program Aborted\n");
					return 0;
				}
				fprintf(fp3, "%s\n", machineCode);
				break;
			case (12) :
				fscanf(fp2,"%d",&operand1);
				validity = validity * getBinaryCodeOfKeyword(root, current_instruction, machineCode);
				validity = validity * appendBinaryCodeOfOperand(machineCode,operand1,12);
				if(validity == -1){
					printf("Invalid Arguments\n");
					printf("Program Aborted\n");
					return 0;
				}
				fprintf(fp3, "%s\n", machineCode);
				break;
			case (16) :
				validity = validity * getBinaryCodeOfKeyword(root, current_instruction, machineCode);
				if(validity == -1){
					printf("Invalid Arguments\n");
					printf("Program Aborted\n");
					return 0;
				}
				fprintf(fp3, "%s\n", machineCode);
				break;
			default:
				break;
		}
	}
	return 0;
}

// getnode() function allotes memory to a node with leaf set as 0.
node* getnode() 
{
	node* new_node;
	new_node = (node*)malloc(sizeof(node));
	new_node->leaf = 0;
	return new_node;
}

// insertKeyword inserts the keyowrd in the TRIE. All the Binary Code of each opcode is
// kept in "opcodes.txt".
node* insertKeyword(node* root, char* keyword, char* binaryCodeOfKeyword)
{
	if(root == NULL){
		root = getnode();
	}
	node* v = root;
	int i = 0;
	while(keyword[i] != '\0'){
		node* temp = v->child[keyword[i] - 'A'];
		if(temp == NULL){
			temp = v->child[keyword[i] - 'A'] = getnode();
		}
		v = temp;
		i++;
	}
	v->leaf = 1;
	strcpy(v->binarycode, binaryCodeOfKeyword);
	return root;
}

// convertDecimalToBinary function converts Decimal number to Binary Representation
// of the same number in 4 - bit format
int convertDecimalToBinary(int number, char* binaryRepresentationOfDecimal)
{
	if(number > 15 || number < 0){
		printf("Operand Value not belongs to [0,15]\n");
		return -1;
	}
	int ind = 0;
	while(number > 0){
		binaryRepresentationOfDecimal[ind] = (number % 2) + '0';
		number >>= 1;
		ind++;
	}
	binaryRepresentationOfDecimal[ind] = '\0';
	
	int i = 0;
	int j = ind - 1;
	for(;i < j; i++,j--){
		char temp = binaryRepresentationOfDecimal[i];
		binaryRepresentationOfDecimal[i] = binaryRepresentationOfDecimal[j];
		binaryRepresentationOfDecimal[j] = temp;
	}
	int len = strlen(binaryRepresentationOfDecimal);
	char bin[4];
	i = 0;
	int k=0;
	for(i = 0;i < 4 - len;i++){
		bin[i] = '0';
	}
	for(i = 4 - len,k = 0;i < 4; i++,k++){
		bin[i] = binaryRepresentationOfDecimal[k];
	}
	strcpy(binaryRepresentationOfDecimal,bin);
	return 1;
}

// getBinaryCodeOfKeyword function retrives the Binary Code of the keyword kept in TRIE
int getBinaryCodeOfKeyword(node* root, char* keyword, char* machineCode)
{
	node* v = root;
	int i = 0;
	while(keyword[i] != '\0'){
		node* temp = v->child[keyword[i] - 'A'];
		if(temp == NULL){
			printf("Invalid Keyword in input\n");
			return -1;
		}
		v = temp;
		i++;
	}
	strcpy(machineCode,v->binarycode);
	return 1;
}

// appendBinaryCodeOfOperand function appends the binary code of opernad in machine code
// appended binary code is of 4 - bit Size
int appendBinaryCodeOfOperand(char* machineCode, int operand_value, int start_from)
{
	char BinarycodeOfDecimal[100];
	int validity = convertDecimalToBinary(operand_value,BinarycodeOfDecimal);
	if(validity == -1){
		return -1;
	}
	int ind = 0;
	for(ind = 0;ind < 4;ind++,start_from++){
		machineCode[start_from] = BinarycodeOfDecimal[ind];
	}
	machineCode[start_from] = '\0';
	printf("%s\n",machineCode);
	return 1;
}
