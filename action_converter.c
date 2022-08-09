#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils/index.h"
#include "./utils.h"

#define LINE_LENGTH 80
typedef struct node *lablePtr;
typedef struct node {
	int line;
	char* name;
	lablePtr next;
} lable;

int write_binary_actions(char*, FILE*);
char* action_converter(char*);
void add_lable_to_table(int, char*, lablePtr);
lablePtr add_front(lablePtr, lablePtr);
lablePtr create_new_lable(int, char*);


char* action_converter(char* file_name) {
	FILE* file_ptr = fopen(file_name, "a+");
	char* new_file_name = replace_file_name_ending(file_name, "_Binary.txt"); // TODO 5 - For debugpurposes should be .am
	FILE* new_fp = fopen(new_file_name, "wt");
	isFilePtrNullish(new_fp);

	char original_line[LINE_LENGTH];
	char copied_line[LINE_LENGTH];
	char* first_word;
	//char* current_lable;
	int line_counter = 100;
	bool is_inside_of_lable = false;
	bool is_lable_word = false;
	lablePtr head = NULL;

	while (fgets(original_line, LINE_LENGTH, file_ptr) != NULL) {

		strcpy(copied_line, original_line);
		first_word = strtok(copied_line, " \n\t");
		is_lable_word = is_lable(first_word);
		if (is_inside_of_lable && !is_lable_word) {
			line_counter += write_binary_actions(original_line, new_fp);
			continue;
		}

		if (is_lable_word) {
			add_lable_to_table(line_counter, first_word, head);
			line_counter += write_binary_actions(original_line, new_fp);
		}
	}
	return "test";
}

int write_binary_actions(char* line, FILE* new_fp) {
	int line_of_binary_actions = 0; // Count the binary lines converted from the original line
	char* operation = strtok(line, " \t\n"); // Get the op code
	if (operation[strlen(operation) - 1] == ':') { // If this is the first time skip the lable
		operation = strtok(NULL, " ");
	}
	// TODO 5 - I assume that the operands looks like this: 'op1,op2' check if there is possibilty of \t or " "
	// another assuming is that there is only two operands need to check it.
	char* first_operand = strtok(NULL, ",");
	char* second_operand = strtok(NULL, "\n");
	
											   
	//char binary_word[10];
	//strcpy(binary_word ,get_binary_word(operation));
	//fprintf(new_fp, "%s", binary_word);
	
	return line_of_binary_actions;
}

//char* get_binary_word(char* word) {
//	if (word[0] == '.') {
//		get_binary_put_action(word);
//	}
//}
//
//
//char* get_binary_put_action(char* word) {
//	char last_letter = word[strlen(word) - 1];
//	switch (last_letter) 
//	{
//		case 't':
//			printf("struct");// statements
//			break;
//
//		case 'g':
//			printf("string");// statements
//			break;	
//	
//		case 'a':
//			printf("data");// statements
//			break;
//
//		default:
//			printf("no match");
//	}
//	return "test";
//}

void add_lable_to_table(int line_counter, char* lable, lablePtr head) {
	lable[strlen(lable)-1] = '\0';
	lablePtr newLable = create_new_lable(line_counter, lable);
	head = add_front(head, newLable);
	printf("Create table for lables");
}

lablePtr add_front(lablePtr head, lablePtr newLable) {
	newLable->next = head;
	return newLable;
}

lablePtr create_new_lable(int line, char* lable_name) {
	lablePtr new = (lablePtr)malloc(sizeof(lable));
	if (new != NULL) {
		strcpy(new->name,lable_name);
		new->line = line;
		new->next = NULL;
	}
	else {
		error_handler("There was an allocating problem");
	}
	return new;
}