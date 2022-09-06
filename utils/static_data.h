#ifndef HEADERFILE_H
#define HEADERFILE_H

#include <stdio.h>
#include <math.h>
#define BASE_NUMBERS (const char[32]){'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'}
#define RGISTERS (const char*[9]){"r0","r1","r2","r3","r4","r5","r6","r7","PSW"}
#define OPCODE (const char*[16]){"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"}
#define BINARY_OPCODE (const char*[16]){"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"}
#define TWO_OPERANDS_ARR (const char*[5]){"mov","cmp","add","sub","lea"}
#define ONE_OPERANDS_ARR (const char*[9]){"not","clr","inc","dec","jmp","bne","get","prn","jsr"}
#define NO_OPERANDS_ARR (const char*[2]){"rts","hlt"}
#define SAVED_WORDS (const char*[5]){"entry","extern", "data", "string", "struct"}

typedef struct node *opPtr;
typedef struct node {
	char* opcode;
	char* bin;
	opPtr next;
} opcode_st;


opPtr init_op_code_list();

#endif