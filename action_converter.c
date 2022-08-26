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

int convert_to_binary_actions(char*, FILE*, opPtr);
bool update_all_missing_parts(lablePtr, externPtr, entryPtr);
char* action_converter(char*);
char* get_ARE_bin(char* word, externPtr, entryPtr, lablePtr);
void add_line_to_entry_or_extern(char*, int, externPtr, entryPtr, char*);

bool is_put_action(char*, char*);
void add_data_to_data(char*);
void add_string_to_data(char*);
void add_struct_to_data(char*);

char* get_binary_opcode(char*, opPtr);
char* get_binary_operand(char*);
char* build_binary_word(char*, char*, char*, char*);
void handle_op(char*, char*, bool is_source);
void handle_reg_operand(char* operand, bool is_source);
void handle_struct_operand(char* operand);
void handle_put_operand(char* operand);
void handle_lable_operand(char* operand);

int IC = 0;
int DC = 0;
// TODO 4 - Change to dynamically allocated array
char I_Array[256][11];
char D_Array[256][11];


char* action_converter(char* file_name) {
	FILE* file_ptr = fopen(file_name, "a+");
	char* new_file_name = replace_file_name_ending(file_name, "_Binary.txt"); // TODO 5 - For debug purposes should be .am
	FILE* new_fp = fopen(new_file_name, "wt");
	isFilePtrNullish(new_fp); // TODO 3 - Rename
	opPtr opcode_header = init_op_code_list();

	char original_line[LINE_LENGTH];
	char copied_line[LINE_LENGTH];
	char* first_word;
	int line_counter = 100;
	int num_of_lines = 0;
	bool is_inside_of_lable = false;
	bool is_lable_word = false;
	lablePtr head = NULL;
	externPtr external_header = NULL;
	entryPtr entry_header = NULL;
	
	while (fgets(original_line, LINE_LENGTH, file_ptr) != NULL) {

		if (original_line[0] == ';') {
			continue; // Ignore comment out lines
		}

		strcpy(copied_line, original_line);
		first_word = strtok(copied_line, " \n\t");
		
		if (is_extern(first_word)) {
			external_header = add_extern_to_table(-1, strtok(NULL, " \t\n"), external_header);
			continue;
		}

		if (is_entry(first_word)) {
			entry_header = add_entry_to_table(-1, strtok(NULL, " \t\n"), entry_header);
			continue;
		}

		is_lable_word = is_lable(first_word);
		if (is_inside_of_lable && !is_lable_word) {
			num_of_lines += convert_to_binary_actions(original_line, new_fp, opcode_header);
			continue;
		}

		if (is_lable_word) {
			line_counter += num_of_lines;
			num_of_lines = 0;
			head = add_lable_to_table(line_counter, first_word, head);
			if (line_counter == 100) {
				line_counter++;
			}
			num_of_lines += convert_to_binary_actions(original_line, new_fp, opcode_header);
			is_inside_of_lable = true;
		}
		else {
			bool is_put = is_put_action(first_word, copied_line);
			if (!is_put) {
				write_error_to_file("Unknown line format");
			}
		}
	}

	
	bool is_errors = update_all_missing_parts(head, external_header, entry_header);
	if (is_errors) {
		exit(0);
	}


	int i = 0;
	printf("\n I_Array \n");
	while (strcmp(I_Array[i], "") != 0) {
		printf("\n %s \n", I_Array[i]);
		i++;
	}
	i = 0;
	printf("\n D_Array \n");
	while (strcmp(D_Array[i], "") != 0) {
		printf("\n %s \n", D_Array[i]);
		i++;
	}

	printf("\n Lables_Table \n");
	while (head != NULL) {
		printf("\n %s - %d \n", head->name, head->line);
		head = head->next;
	}

	printf("\n Extern_Table \n");
	while (external_header != NULL) {
		printf("\n %s - %d \n", external_header->name, external_header->line);
		external_header = external_header->next;
	}

	printf("\n Entry_Table \n");
	while (entry_header != NULL) {
		printf("\n %s - %d \n", entry_header->name, entry_header->line);
		entry_header = entry_header->next;
	}

	return "test";
}

bool update_all_missing_parts(lablePtr lable_head, externPtr extern_head, entryPtr entry_head) {
	int line;
	int i = 0;
	char* ARE;
	char* bin_line;

	while (strcmp(I_Array[i], "") != 0) {
		if ( is_letter(I_Array[i][0]) ) {
			line = get_line_of_lable(I_Array[i], lable_head);
			ARE = get_ARE_bin(I_Array[i], extern_head, entry_head, lable_head); 
			bin_line = convert_dec_to_bin(line, false);
			strcat(bin_line, ARE);
			add_line_to_entry_or_extern(I_Array[i], (i+100), extern_head, entry_head, ARE, lable_head);
			memcpy(I_Array[i], bin_line, strlen(bin_line));

		}
		i++;
	}

	while (extern_head != NULL && entry_head != NULL) {

		if (entry_head->line == -1 && extern_head->line == -1) {
			write_error_to_file("Could Not find the entry lable line");
			return true;
		}

		extern_head = (extern_head == NULL) ? NULL : extern_head->next;
		entry_head = (entry_head == NULL) ? NULL : entry_head->next;
	}

	return false;
}


void add_line_to_entry_or_extern(char* lable, int line, externPtr extern_head, entryPtr entry_head, char* ARE, lablePtr lable_head) {
	entryPtr temp_entry = entry_head;
	externPtr temp_extern = extern_head;
	int line_from_lable_table;
	if (strcmp(ARE, "01") == 0) {
		while (extern_head != NULL) {
			if (strcmp(lable, extern_head->name) == 0) {
				if (extern_head->line != -1) {
					extern_head = add_extern_to_table(line, extern_head->name, temp_extern);
					break;
				}
				else {
					extern_head->line = line;
					break;
				}
			}
			extern_head = extern_head->next;
		}
	}

	if (strcmp(ARE, "10") == 0) {
		while (entry_head != NULL) {
			if (strcmp(lable, entry_head->name) == 0) {
				line_from_lable_table = get_line_of_lable(entry_head->name, lable_head);
				if ( (entry_head->line != -1) && (line_from_lable_table == NULL) ) {
					entry_head = add_entry_to_table(line, entry_head->name, temp_entry);
					break;
				}
				else {
					entry_head->line = (entry_head->line != -1) ? line : line_from_lable_table;
					break;
				}
			}

			entry_head = entry_head->next;
		}
	}
}

int get_line_of_lable(char* name, lablePtr lable_head) {
	while (lable_head != NULL) {
		if (strcmp(name, lable_head->name) == 0) {
			return lable_head->line;
		}
		lable_head = lable_head->next;
	}

	return NULL;
}

char* get_ARE_bin(char* word, externPtr extern_head, entryPtr entry_head, lablePtr lable_head) {
	while (extern_head != NULL && entry_head != NULL) {
	
		if (strcmp(extern_head->name, word) == 0) {
			return "01";
		}
		if (strcmp(entry_head->name, word) == 0 || strcmp(lable_head->name, word) == 0) {
			return "10";
		}
		extern_head = (extern_head == NULL) ? NULL : extern_head->next;
		entry_head = (entry_head == NULL) ? NULL : entry_head->next;
		lable_head = (lable_head == NULL) ? NULL : lable_head->next;
	}
	return "00";
}

int convert_to_binary_actions(char* line, FILE* new_fp, opPtr opcode_header) {
	char copied_line[LINE_LENGTH];
	strcpy(copied_line, line);
	int line_of_binary_actions = (IC + DC); // Count the binary lines converted from the original line
	char* operation = strtok(line, " \t\n"); // Get the op code
	if (operation[strlen(operation) - 1] == ':') { // If this is the first time skip the lable
		operation = strtok(NULL, " ");
	}
	if (operation[0] == '.' && is_put_action(operation, copied_line)) { // If it is it puts the data (the name 'is_put_action' is misleading)
		return (IC + DC) - line_of_binary_actions;
	}
	int num_of_op = get_num_of_op_by_operation(operation);
	char* source_op;
	char* destination_op;
	char* binary_source_op = "00";
	char* binary_destination_op = "00";
	char* binary_opcode = get_binary_opcode(operation, opcode_header);
	if (IC > 0) {
		IC++;
	}
	if (num_of_op == 2) {
		source_op = strtok(NULL, ", \t\n");
		destination_op = strtok(NULL, ", \t\n");
		binary_source_op = get_binary_operand(source_op);
		binary_destination_op = get_binary_operand(destination_op);
		strcpy(I_Array[IC], build_binary_word(binary_opcode, binary_source_op, binary_destination_op, "00"));
		bool is_both_regs = (binary_source_op == "11" && binary_destination_op == "11");
		if (is_both_regs) {
			IC++;
			strcpy(I_Array[IC], strcat(concat(get_binary_reg(source_op), get_binary_reg(destination_op)), "00")); // Add the two regs to the binary num
		}
		else {
			handle_op(source_op, binary_source_op, true);
			handle_op(destination_op, binary_destination_op, false);
		}
	}
	else if (num_of_op == 1) {
		destination_op = strtok(NULL, " \t\n");
		binary_destination_op = get_binary_operand(destination_op);
		strcpy(I_Array[IC], build_binary_word(binary_opcode, binary_source_op, binary_destination_op, "00")); // Binary words, the ARE data
		handle_op(destination_op, binary_destination_op, false);
	}
	else {
		strcpy(I_Array[IC], build_binary_word(binary_opcode, binary_source_op, binary_destination_op, "00")); // Binary words, the ARE data
	}
	
	return (IC + DC) - line_of_binary_actions;
}

char* build_binary_word(char* op, char* source, char* des, char* A_R_E) { // TODO 3 - Find better solution
	char* bin_word = op;
	bin_word = concat(bin_word, source);
	bin_word = concat(bin_word, des);
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

// Kind of factory pattern to choose what to do with the operand
void handle_op(char* operand, char* bin_op, bool is_source) {
	if (!operand) { error_handler("oops something went wrong! (handle_source_op)"); }
	if (strcmp(bin_op, "10") == 0) {
		handle_struct_operand(operand);
	}
	if (strcmp(bin_op, "00") == 0) {
		handle_put_operand(operand);
	}
	if (strcmp(bin_op, "11") == 0) {
		handle_reg_operand(operand, is_source);
	}
	if (strcmp(bin_op, "01") == 0) {
		handle_lable_operand(operand);
	}
}

void handle_struct_operand(char* operand) {
	char* struct_lable = strtok(operand, ".");
	char* num = strtok(NULL, ".");
	IC++;
	strcpy(I_Array[IC], struct_lable);
	if (num[0] < '1' || num[0] > '2') {
		write_error_to_file("The struct places has only 1 and 2"); // Deliver line number
	}
	IC++;
	strcpy(I_Array[IC], (num[0] == '1') ? "0000000100" : "0000001000");
}

void handle_put_operand(char* operand) {
	char* num = memmove(operand, operand + 1, strlen(operand));
	bool is_negative = (num[0] == '-');
	if (is_negative) {
		memmove(num, num + 1, strlen(num));
	}
	IC++;
	if (is_valid_number(num)) {
		strcpy(I_Array[IC], convert_dec_to_bin10(atoi(num), is_negative));
	}
	else {
		write_error_to_file("Number is not valid");
		strcpy(I_Array[IC], "error");
	}
}

void handle_reg_operand(char* operand, bool is_source) {
	char* binary_reg = is_source ? concat(get_binary_reg(operand), "0000") : concat("0000", get_binary_reg(operand));
	IC++;
	strcpy(I_Array[IC], concat(binary_reg, "00"));
}

void handle_lable_operand(char* operand) {
	IC++;
	strcpy(I_Array[IC], operand);
}

//TODO 4 - Separate this func to action and bool info cause the name is missleading.
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
			write_error_to_file("No match for the data");
			strcpy(D_Array[DC], "error");
			is_put = false;
	}
	return is_put;
}

void add_data_to_data(char* line) {
	int counter = 0;
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == ',') counter++;
	}
	char* lable = strtok(line, " /t/n");
	char* data = strtok(NULL, " ");
	char* num = strtok(NULL, " ,/t/n");
	int num_counter = 0;

	while (num != NULL && (strcmp(num, "\n") != 0) ) { // We refer to char also as a number
		num_counter++;
		
		bool is_negative = (num[0] == '-');
		if (is_negative) {
			memmove(num, num + 1, strlen(num));
		}
		if (is_valid_number(num)) {
			strcpy(D_Array[DC], convert_dec_to_bin10(atoi(num), is_negative));
		}
		else {
			write_error_to_file(("%s is not valid number", num)); // TODO 5 - Add pointer to error file
			strcpy(D_Array[DC], "error");
		}
		DC++;
		num = strtok(NULL, " ,/t/n");
	}
	if (counter >= num_counter) {
		error_handler("Too many (,) symbols (.data op)");
	}
}


void add_struct_to_data(char* line) {
	char* lable = strtok(line, " /t/n");
	char* struct_lable = strtok(NULL, " ");
	char* num = strtok(NULL, " ,/t/n");
	char* str = strtok(NULL, " /t/n");
	
	bool is_negative = (num[0] == '-');
	if (is_negative) {
		memmove(num, num + 1, strlen(num));
	}
	strcpy(D_Array[DC], convert_dec_to_bin10(atoi(num), is_negative));
	bool flag = false;
	for (int i = 0; i < strlen(str); i++) {
		if (line[i] == '\"') {
			flag = !flag;
			if (DC > 0 && flag) {
				DC++;
			}
		}
		if (flag) {
			strcpy(D_Array[DC], convert_dec_to_bin10(str[i], false)); // we send here a char as integer
			DC++;
		}
	}
	strcpy(D_Array[DC], "0000000000");
	DC++;
}

void add_string_to_data(char* line) {
	bool flag = false;
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == '\"') {
			flag = !flag;
			i++;
		}
		if (flag) {
			strcpy(D_Array[DC], convert_dec_to_bin10(line[i], false)); // we send here a char as integer
			DC++;
		}
	}
	strcpy(D_Array[DC], "0000000000");
	DC++;
}

// TODO 5 - Check for double lable names
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
	entryPtr new_entry_lable = create_new_entry(line_counter, entry_lable);
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
	externPtr new_extern_lable = create_new_extern(line_counter, extern_lable);
	head = add_front_extern(new_extern_lable, head);
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
