#include <stdio.h>
#include <math.h>
#define BASE_NUMBERS (const char[32]){'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'}
#define OPCODE (const char*[16]){"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"}
#define BINARY_OPCODE (const char*[16]){"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"}
#define CODING_TYPE(x,y) ((x)==0 && (y)==0 ? 'A' : ((x)==0 && (y)==1 ? 'E' : ((x)==1 && (y)==0 ? 'R' : 'N')))

typedef struct node *opPtr;
typedef struct node {
	char* opcode;
	char* bin;
	opPtr next;
} opcode_st;
/*
typedef struct node *base32Ptr;
typedef struct node {
	char* num;
	char* bin;
	base32Ptr next;
} base32_st;
*/

opPtr init_op_code_list();
//base32Ptr init_base32_list();
