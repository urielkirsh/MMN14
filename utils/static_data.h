#include <stdio.h>
#include <math.h>
#define BASE_NUMBERS (const char[32]){'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'}
#define OPCODE (const char*[16]){"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"}
#define CODING_TYPE(x,y) ((x)==0 && (y)==0 ? 'A' : ((x)==0 && (y)==1 ? 'E' : ((x)==1 && (y)==0 ? 'R' : 'N')))
#define GET_INDEX(x)\
          for (int i = 0; i < 32; i++) { \
            if((x) == BASE_NUMBERS[i]) return i\
          }

#define CREATE_BINARY_NUMBER(x,y,z,w) ((x)*1000+(y)*100+(z)*10+(w))

#define GET_ACTION(index) OPCODE[(index)]

int convert(long long);
int get_index(int i);

int convert(long long n) {
	int dec = 0, i = 0, rem;

	while (n != 0) {
		rem = n % 10;
		n /= 10;
		dec += rem * pow(2, i);
		++i;
	}

	return dec;
}

//int get_index(int n) {
//	int index = -1;
//	for (int i = 0; i < 32; i++) {
//		if (n == OPCODE[i]) {
//			index = i;
//			break;
//		}
//	}
//
//	if (index > -1) {
//		printf("Index : %d\n", index);
//	}
//	else {
//		printf("%d is not present in this array.\n", n);
//		return 0;
//	}
//	return index;
//}