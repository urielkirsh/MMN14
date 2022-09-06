#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./utils/static_data.h"
#include "utils/index.h"
#include "./utils.h"
#include "action_converter_linked_list_helper.h"
#include "action_converter_helper.h"

#define LINE_LENGTH 80

int convert_to_binary_actions(char*, opPtr);
bool update_all_missing_parts(lablePtr, externPtr, entryPtr, char*);
char* action_converter(char*);
char* get_ARE_bin(char* word, externPtr, entryPtr, lablePtr);
externPtr add_line_to_extern(char*, int, externPtr, lablePtr);
entryPtr add_line_to_entry(char*, int, entryPtr, lablePtr);

bool is_put_action(char*, char*);
void add_data_to_data(char*);
void add_string_to_data(char*);
void add_struct_to_data(char*);


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

// TODO 4 - create macro to free lists
char* action_converter(char* file_name) {

	FILE* file_ptr = fopen(file_name, "a+");
	
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
			num_of_lines += convert_to_binary_actions(original_line, opcode_header);
			continue;
		}

		if (is_lable_word) {
			line_counter += num_of_lines;
			num_of_lines = 0;
			head = add_lable_to_table(line_counter, first_word, head);
			if (line_counter == 100) {
				line_counter++;
			}
			num_of_lines += convert_to_binary_actions(original_line, opcode_header);
			is_inside_of_lable = true;
		}
		else {
			bool is_put = is_put_action(first_word, copied_line);
			if (!is_put) {
				error_handler("Unknown line format");
			}
		}
	}

	
	bool is_errors = update_all_missing_parts(head, external_header, entry_header, file_name);
	if (is_errors) {
		exit(0);
	}

	char* new_file_name = replace_file_name_ending(file_name, "_Binary.txt"); // TODO 5 - For debug purposes should be .am
	FILE* new_fp = fopen(new_file_name, "wt");
	isFilePtrNullish(new_fp); // TODO 3 - Rename

	char bin_IC[3]; 
	strcpy(bin_IC, convert_dec_to_32b(IC+1, "NULL", 10));
	char bin_DC[3];
	strcpy(bin_DC, convert_dec_to_32b(DC, "NULL", 10));
	fprintf(new_fp, "%c%c%c%c%c%c", bin_IC[0], bin_IC[1], ' ', bin_DC[0], bin_DC[1], '\n');
	
	char bin32[3];
	char binLine[3];
	int i = 0;
	int line;

	while (strcmp(I_Array[i], "") != 0) {
		line = i + 100;
		strcpy(binLine, convert_dec_to_32b(line, "NULL", 10));
		strcpy(bin32, convert_dec_to_32b(-1, I_Array[i], 10));
		i++;
		fprintf(new_fp, "%c%c%c%c%c%c", binLine[0], binLine[1], '\t', bin32[0], bin32[1], '\n');
	}
	
	i = 0;
	while (strcmp(D_Array[i], "") != 0) {
		strcpy(bin32, convert_dec_to_32b(-1, D_Array[i], 10));
		line = i + 101 + IC;
		strcpy(binLine, convert_dec_to_32b(line, "NULL", 10));
		fprintf(new_fp, "%c%c%c%c%c%c", binLine[0], binLine[1], '\t', bin32[0], bin32[1], '\n');
		i++;
	}

	if (new_fp) {
		fclose(new_fp);
	}

	return "test";
}

bool update_all_missing_parts(lablePtr lable_head, externPtr extern_head, entryPtr entry_head, char* file_name) {
	FILE* entry_fp;
	FILE* extern_fp;
	int line;
	int i = 0;
	int line_from_lable_table;
	char* ARE;
	char* bin_line;
	char lable[10]; // Assuming that the lable is not bigger than 10 chars
	char entry_bin32[3];
	char extern_bin32[3];


	while (strcmp(I_Array[i], "") != 0) {
		if ( is_letter(I_Array[i][0]) ) {
			if (strcmp(I_Array[i], "error") == 0) {
				return true;
			}
			strcpy(lable, I_Array[i]);
			line = get_line_of_lable(I_Array[i], lable_head);
			ARE = get_ARE_bin(I_Array[i], extern_head, entry_head, lable_head); 
			bin_line = convert_dec_to_bin(line, false);
			strcat(bin_line, ARE);
			memcpy(I_Array[i], bin_line, strlen(bin_line));

			if (strcmp(ARE, "01") == 0) {
				extern_head = add_line_to_extern(lable, (i+100), extern_head, lable_head);
			}
			if (strcmp(ARE, "10") == 0) {
				entry_head = add_line_to_entry(lable, (i+100), entry_head, lable_head);
			}
		}
		i++;
	}

	// TODO 4 - Move all the entry/extern writing to a proper function
	if (extern_head) {
		char* new_file_name = replace_file_name_ending(file_name, "_EXT.txt");
		extern_fp = fopen(new_file_name, "w+");
		isFilePtrNullish(extern_fp);
	}

	if (entry_head) {
		char* new_file_name = replace_file_name_ending(file_name, "_ENT.txt");
		entry_fp = fopen(new_file_name, "w+");
		isFilePtrNullish(entry_fp);
	}

	while (extern_head != NULL || entry_head != NULL) {


		if (entry_head) {
			if (entry_head->line == -1) {
				line_from_lable_table = get_line_of_lable(entry_head->name, lable_head);
				entry_head->line = line_from_lable_table ? line_from_lable_table : -1;
			}

			if (entry_head->line != -1) {
				strcpy(entry_bin32, convert_dec_to_32b(entry_head->line, "NULL", 10));
				fprintf(entry_fp, "%s%c%c%c%c", entry_head->name, '\t', entry_bin32[0], entry_bin32[1], '\n');
			}
		}

		if (extern_head) {
			if (extern_head->line == -1) {
				line_from_lable_table = get_line_of_lable(extern_head->name, lable_head);
				extern_head->line = line_from_lable_table ? line_from_lable_table : -1;
			}
			if (extern_head->line != -1) {
				strcpy(extern_bin32, convert_dec_to_32b(extern_head->line, "NULL", 10));
				fprintf(extern_fp, "%s%c%c%c%c", extern_head->name, '\t', extern_bin32[0], extern_bin32[1], '\n');

			}
		}

		if ((entry_head && entry_head->line == -1) || (extern_head && extern_head->line == -1)) {
			error_handler("Could Not find the entry/extern lable line");
			return true;
		}

		extern_head = (extern_head == NULL) ? NULL : extern_head->next;
		entry_head = (entry_head == NULL) ? NULL : entry_head->next;
		
		if (entry_head == NULL) { // Finish writing to the file
			fclose(entry_fp);
		}

		if (extern_head == NULL) { // Finish writing to the file
			fclose(extern_fp);
		}
	}


	return false;
}


externPtr add_line_to_extern(char* lable, int line, externPtr extern_head, lablePtr lable_head) {
	
	externPtr temp_extern = extern_head;

	while (extern_head != NULL) {
		if (strcmp(lable, extern_head->name) == 0) {
			if (extern_head->line != -1) {
				temp_extern = add_extern_to_table(line, extern_head->name, temp_extern);
				break;
			}
			else {
				extern_head->line = line;
				break;
			}
		}
		extern_head = extern_head->next;
	}
	return temp_extern;
}

entryPtr add_line_to_entry(char* lable, int line, entryPtr entry_head, lablePtr lable_head) {
	
	entryPtr temp_entry = entry_head;
	int line_from_lable_table;
	
	while (entry_head != NULL) {
		if (strcmp(lable, entry_head->name) == 0) {
			if (entry_head->line != -1) {
				temp_entry = add_entry_to_table(line, entry_head->name, temp_entry);
				break;
			}
			else {
				entry_head->line = line;
			}
		}
		else {
			line_from_lable_table = get_line_of_lable(entry_head->name, lable_head);
			entry_head->line = line_from_lable_table;
			break;
		}

		entry_head = entry_head->next;
	}
	return temp_entry;
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
	while (extern_head != NULL || entry_head != NULL || lable_head != NULL) {
	
		if (extern_head && strcmp(extern_head->name, word) == 0) {
			return "01";
		}
		if (( entry_head && strcmp(entry_head->name, word) == 0 ) || (lable_head && strcmp(lable_head->name, word) == 0)) {
			return "10";
		}
		extern_head = (extern_head == NULL) ? NULL : extern_head->next;
		entry_head = (entry_head == NULL) ? NULL : entry_head->next;
		lable_head = (lable_head == NULL) ? NULL : lable_head->next;
	}
	return "00";
}

int convert_to_binary_actions(char* line, opPtr opcode_header) {
	char copied_line[LINE_LENGTH];
	strcpy(copied_line, line);
	int line_of_binary_actions = (IC + DC); // Count the binary lines converted from the original line
	char* operation = strtok(line, " \t\n"); // Get the op code
	if (operation[strlen(operation) - 1] == ':') { // If this is the first time skip the lable
		operation = strtok(NULL, " \t\n");
	}
	if (operation[0] == '.' && is_put_action(operation, copied_line)) { // TODO 4 - Separate. The method also puts the data (the name 'is_put_action' is misleading)
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

	int current_num_of_op = get_num_of_op_by_operands_by_line(copied_line);
	if (num_of_op != current_num_of_op) {
		char error[50] = "Too many operands for operation ";
		strcat(error, operation);
		write_error_to_file(error, IC);
	}
	
	return (IC + DC) - line_of_binary_actions;
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
		write_error_to_file("The struct places has only 1 and 2", IC); // Deliver line number
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
		strcpy(I_Array[IC], strcat(convert_dec_to_bin(atoi(num), is_negative), "00"));
	}
	else {
		write_error_to_file("Number is not valid", IC);
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
			write_error_to_file("No match for the data", DC+IC);
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
			write_error_to_file(("%s is not valid number", num), DC + IC); // TODO 5 - Add pointer to error file
			strcpy(D_Array[DC], "error");
		}
		DC++;
		num = strtok(NULL, " ,/t/n");
	}
	if (counter >= num_counter) {
		write_error_to_file("Too many (,) symbols (.data op)", DC + IC);
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
		if (str[i] == '\"') {
			flag = !flag;
			if (DC > 0 && flag) {
				DC++;
			}
		}
		if (flag && (str[i] != '\"')) {
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
