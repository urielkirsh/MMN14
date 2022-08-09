#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/static_data.h"

opPtr create_opcode_struct(int);
opPtr add_new_opcode(opPtr, opPtr);

opPtr init_op_code_list() {
	opPtr head = NULL;
	int i = 0;
	for (i; i < 16; i++) {
		opPtr new_opcode_struct = create_opcode_struct(i);
		head = add_new_opcode(new_opcode_struct, head);
	}
	return head;
}

opPtr create_opcode_struct(int i) {
	opPtr new = (opPtr)malloc(sizeof(opcode_st));
	if (new != NULL) {
		new->opcode = OPCODE[i];;
		new->bin = BINARY_OPCODE[i];
		new->next = NULL;
	}
	else {
		printf("There was an allocating problem");
		exit(0);
	}
	return new;
}

opPtr add_new_opcode(opPtr new_opcode_struct, opPtr head) {
	new_opcode_struct->next = head;
	return new_opcode_struct;
}