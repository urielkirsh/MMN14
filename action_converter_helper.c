#include <stdio.h>
#include "./utils.h"
#include "action_converter_helper.h"


char* build_binary_word(char* op, char* source, char* des, char* A_R_E) { // TODO 3 - Find better solution
	char* bin_word = op;
	bin_word = concat(bin_word, source);
	bin_word = concat(bin_word, des);
	bin_word = concat(bin_word, A_R_E);
	return bin_word;
}

char* get_binary_operand(char* source_op) {
	if (source_op[0] == '#') { // Assuming that the op looks like this: #...rest
		return "00";
	}
	if (source_op[strlen(source_op) - 2] == '.') { // Assuming that the op of struct looks like this: X.1 or X.2
		return "10";
	}
	if (is_reg(source_op)) { // Checks if this is a register name
		return "11";
	}
	return "01";
}

char* get_binary_opcode(char* operation, opPtr opcode_header) {
	opPtr current = opcode_header;
	while (current != NULL)
	{
		if (strcmp(current->opcode, operation) == 0) {
			return current->bin;
		}
		current = current->next;
	}
	return "";
}