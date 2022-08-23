#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./utils/static_data.h"
#include "utils/index.h"
#include "./utils.h"

#define LINE_LENGTH 80
typedef struct lable_node *lablePtr;
typedef struct lable_node {
	int line;
	char name[10];
	lablePtr next;
} lable;

typedef struct entry_node* entryPtr;
typedef struct entry_node {
	int line;
	char name[10];
	entryPtr next;
} entry;

typedef struct extern_node* externPtr;
typedef struct extern_node {
	int line;
	char name[10];
	externPtr next;
} external;

lablePtr add_lable_to_table(int, char*, lablePtr);
lablePtr add_front(lablePtr, lablePtr);
lablePtr create_new_lable(int, char*);

entryPtr add_entry_to_table(int, char*, entryPtr);
entryPtr add_front_entry(entryPtr, entryPtr);
entryPtr create_new_entry(int, char*);

externPtr add_extern_to_table(int, char*, externPtr);
externPtr add_front_extern(externPtr, externPtr);
externPtr create_new_extern(int, char*);

int write_binary_actions(char*, FILE*, opPtr);
char* action_converter(char*);

bool is_put_action(char*, char*);
void add_data_to_data(char*);
void add_string_to_data(char*);
void add_struct_to_data(char*);

char* get_binary_opcode(char*, opPtr);
char* get_binary_operand(char*);
char* build_binary_word(char*, int, char*);
char* handle_source_op(char*);
void handle_des_op(char*);
void init_arrays();
void free_arrays();

int IC = 0;
int DC = 0;
char **I_Array;
char **D_Array;

void init_arrays () {
	I_Array = (char*)malloc(256 * sizeof(char*));
	D_Array = (char*)malloc(256 * sizeof(char*));
}
void free_arrays() {
	free(I_Array);
	free(D_Array);
}

char* action_converter(char* file_name) {
	init_arrays();
	FILE* file_ptr = fopen(file_name, "a+");
	char* new_file_name = replace_file_name_ending(file_name, "_Binary.txt"); // TODO 5 - For debug purposes should be .am
	FILE* new_fp = fopen(new_file_name, "wt");
	isFilePtrNullish(new_fp); // TODO 3 - Rename
	opPtr opcode_header = init_op_code_list();

	char original_line[LINE_LENGTH];
	char copied_line[LINE_LENGTH];
	char* first_word;
	//char* current_lable;
	int line_counter = 100;
	int num_of_lines = 0;
	bool is_inside_of_lable = false;
	bool is_lable_word = false;
	lablePtr head = NULL;
	externPtr external_header = NULL;
	entryPtr entry_header = NULL;
	
	while (fgets(original_line, LINE_LENGTH, file_ptr) != NULL) {

		strcpy(copied_line, original_line);
		first_word = strtok(copied_line, " \n\t");
		
		if (is_extern(first_word)) {
			external_header = add_extern_to_table(-1, strtok(copied_line, " \t"), external_header);
			continue;
		}

		if (is_entry(first_word)) {
			entry_header = add_entry_to_table(-1, strtok(copied_line, " \t"), entry_header);
			continue;
		}

		is_lable_word = is_lable(first_word);
		if (is_inside_of_lable && !is_lable_word) {
			num_of_lines += write_binary_actions(original_line, new_fp, opcode_header);
			continue;
		}

		if (is_lable_word) {
			line_counter += num_of_lines;
			num_of_lines = 0;
			head = add_lable_to_table(line_counter, first_word, head);
			num_of_lines += write_binary_actions(original_line, new_fp, opcode_header);
			is_inside_of_lable = true;
		}
		else {
			bool is_put = is_put_action(first_word, copied_line);
			if (!is_put) {
				write_error_to_file("Unknown line format");
			}
		}
	}

	while (head != NULL){
		printf("%s \n", head->name);
		head = head->next;
	}
	free_arrays();
	return "test";
}

int write_binary_actions(char* line, FILE* new_fp, opPtr opcode_header) {
	int line_of_binary_actions = 0; // Count the binary lines converted from the original line
	char* operation = strtok(line, " \t\n"); // Get the op code
	if (operation[strlen(operation) - 1] == ':') { // If this is the first time skip the lable
		operation = strtok(NULL, " ");
	}
	int num_of_op = get_num_of_op_by_operation(operation);
	char* source_op;
	char* destination_op;
	char* binary_source_op = "00";
	char* binary_destination_op = "00";
	char* binary_opcode = get_binary_opcode(operation, opcode_header);
	if (num_of_op == 2) {
		source_op = strtok(NULL, ", \t\n");
		destination_op = strtok(NULL, ", \t\n");
		binary_source_op = get_binary_operand(source_op);
		binary_destination_op = get_binary_operand(destination_op);
		handle_source_op(source_op);
		handle_des_op(destination_op);
	}
	else if (num_of_op == 1) {
		destination_op = strtok(NULL, " \t\n");
		binary_destination_op = get_binary_operand(destination_op);
		handle_des_op(destination_op);
	}
	char* binary_words[3] = { binary_opcode, binary_source_op, binary_destination_op };
	char* first_binary_word = build_binary_word(binary_words, 3, "00"); // Array of words, num of words, the ARE data
	
	
	//char binary_word[10];
	//strcpy(binary_word ,get_binary_word(operation));
	//fprintf(new_fp, "%s", binary_word);
	
	return line_of_binary_actions;
}

char* build_binary_word(char* words[], int size, char* A_R_E) {
	int i = 1;
	char* bin_word = words[0];
	for (i; i < size; i++) {
		bin_word = concat(bin_word, words[i]);
	}
	bin_word = concat(bin_word, A_R_E);
	return bin_word;
}

char* get_binary_operand (char* source_op) {
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

char* get_binary_opcode (char* operation, opPtr opcode_header) {
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

char* handle_source_op(char* operand) {
	if (!operand) { error_handler("oops something went wrong! (handle_source_op)"); }
	if (is_lable_name(operand)) {
		char* struct_lable = strtok(operand, ".");
		if (struct_lable) {
			char* num = strtok(NULL, ".");
			if (num[0] < '1' || num[0] > '2') {
				error_handler("The struct places has only 1 and 2");
			}
			// IC++
			// i_Array[IC] = struct lable address + ARE check entry or external
			// IC++
			// i_Array[IC] = number + "00"
		}
		else if (is_reg(operand)) {
			// IC++
			// i_Array[IC] = bin reg
		}
	}

	return "test";
}

void handle_des_op(char* destination) {
	printf("%s", destination);
}

//char* get_binary_word(char* word) {
//	if (word[0] == '.') {
//		get_binary_put_action(word);
//	}
//}
//
//
bool is_put_action(char* word, char* line) {
	char last_letter = word[strlen(word) - 1];
	bool is_put = false;
	switch (last_letter) 
	{
		case 't':
			add_struct_to_data(line);
			is_put = true;
			break;

		case 'g':
			add_string_to_data(line);
			is_put = true;
			break;	
	
		case 'a':
			add_data_to_data(line);
			is_put = true;
			break;

		default:
			is_put = false;
	}
	return is_put;
}

void add_data_to_data(char* line) {
	int counter = 0;
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == ',') counter++;
	}
	char* num = strtok(line, " ,/t/n");
	int num_counter = 0;

	while (num != NULL) { // We refer to char also as a number
		num_counter++;
		DC++;
		bool is_negative = (num[0] == '-');
		if (is_negative) {
			memmove(num, num + 1, strlen(num));
		}
		if (is_valid_number(num)) {
			strcpy(D_Array[DC], convert_dec_to_bin(atoi(num), is_negative));
		}
		else {
			write_error_to_file(("%s is not valid number", num)); // TODO 5 - Add pointer to error file
			strcpy(D_Array[DC], "error");
		}

		num = strtok(NULL, " ,/t/n");
	}
	if (counter >= num_counter) {
		error_handler("Too many (,) symbols (.data op)");
	}
}

void add_struct_to_data(char* line) {
	char* num = strtok(line, " ,/t/n");
	char* str = strtok(NULL, " /t/n");
	DC++;
	bool is_negative = (num[0] == '-');
	if (is_negative) {
		memmove(num, num + 1, strlen(num));
	}
	strcpy(D_Array[DC], convert_dec_to_bin(atoi(num), is_negative));
	for (int i = 0; i < strlen(str); i++) {
		DC++;
		strcpy(D_Array[DC], convert_dec_to_bin(str[i], false)); // we send here a char as integer
	}
	DC++;
	strcpy(D_Array[DC], "0000000000");
}

void add_string_to_data(char* line) {
	for (int i = 0; i < strlen(line); i++) {
		DC++;
		strcpy(D_Array[DC], convert_dec_to_bin(line[i], false)); // we send here a char as integer
	}
	DC++;
	strcpy(D_Array[DC], "0000000000");
}

lablePtr add_lable_to_table(int line_counter, char* lable, lablePtr head) {
	lable[strlen(lable)-1] = '\0';
	lablePtr newLable = create_new_lable(line_counter, lable);
	head = add_front(head, newLable);
	printf("Create table for lables");
	return head;
}

lablePtr add_front(lablePtr head, lablePtr newLable) {
	newLable->next = head;
	return newLable;
}

lablePtr create_new_lable(int line, char* lable_name) {
	lablePtr new_lable = (lablePtr)malloc(sizeof(lable));
	if (new_lable != NULL) {
		strcpy(new_lable->name,lable_name);
		new_lable->line = line;
		new_lable->next = NULL;
	}
	else {
		error_handler("There was an allocating problem");
	}
	return new_lable;
}

entryPtr add_entry_to_table(int line_counter, char* entry_lable, entryPtr head) {
	entryPtr new_entry_lable = create_new_lable(line_counter, entry_lable);
	head = add_front_entry(head, new_entry_lable);
	return head;
}

entryPtr add_front_entry(entryPtr head, entryPtr new_entry_lable) {
	new_entry_lable->next = head;
	return new_entry_lable;
}

entryPtr create_new_entry(int line, char* lable_name) {
	entryPtr new_entry_lable = (entryPtr)malloc(sizeof(entry));
	if (new_entry_lable != NULL) {
		strcpy(new_entry_lable->name, lable_name);
		new_entry_lable->line = line;
		new_entry_lable->next = NULL;
	}
	else {
		error_handler("There was an allocating problem");
	}
	return new_entry_lable;
}

externPtr add_extern_to_table(int line_counter, char* extern_lable, externPtr head) {
	externPtr new_extern_lable = create_new_lable(line_counter, extern_lable);
	head = add_front_extern(head, new_extern_lable);
	return head;
}

externPtr add_front_extern(externPtr new_extern_lable, externPtr head) {
	new_extern_lable->next = head;
	return new_extern_lable;
}

externPtr create_new_extern(int line, char* lable_name) {
	externPtr new_extern_lable = (externPtr)malloc(sizeof(external));
	if (new_extern_lable != NULL) {
		strcpy(new_extern_lable->name, lable_name);
		new_extern_lable->line = line;
		new_extern_lable->next = NULL;
	}
	else {
		error_handler("There was an allocating problem");
	}
	return new_extern_lable;
}
