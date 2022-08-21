#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils/index.h"
#include "utils/static_data.h"
#include "./utils.h"
#include "./macro_spreader.h"
#define LINE_LENGTH 80



char* create_spread_macro_file(char* file_name) {
	FILE* file_ptr;
	file_ptr = fopen(file_name, "r");
	isFilePtrNullish(file_ptr);
	return spread_macro(file_ptr, file_name);
}


/*first file's reading*/
char* spread_macro(FILE* file_ptr, char* file_name)
{
	Macro* head = NULL;
	Macro* macro_node = NULL;
	char* new_file_name = replace_file_name_ending(file_name, "_AM.txt"); // TODO 5 - For debugpurposes should be .am 
	FILE* new_fp = fopen(new_file_name, "a+");
	isFilePtrNullish(new_fp);
	int macro = 0;
	char* macro_name;
	char* first_word;
	char* str_to_put = (char*)malloc(sizeof(char) * LINE_LENGTH);
	char original_line[LINE_LENGTH];
	char copied_line[LINE_LENGTH];

	while (fgets(original_line, LINE_LENGTH, file_ptr) != NULL) {

		if (original_line[0] == ';') continue; // Ignore comment out lines

		strcpy(copied_line, original_line);

		if (macro) {
			if (strcmp(original_line, "endmacro\n") == 0) {
				macro = 0;
				continue;
			}
			else {
				strcpy(head->macro_data,concat(head->macro_data, copied_line));
				continue;
			}
		}

		first_word = strtok(copied_line, " \n\t");


		if (strcmp(first_word, "macro") == 0) {
			macro_name = strtok(NULL, " \n");
			macro_node = create_macro(macro_name);
			head = add_macro(head, macro_node);
			macro = 1;
		}
		else {
			if (copied_line[strlen(first_word) - 1] == ':' || is_opcode(first_word)) {
				strncpy(str_to_put, original_line, LINE_LENGTH);
			}
			else {
				strncpy(str_to_put, get_macro(first_word, head), LINE_LENGTH);
			}
			fprintf(new_fp, "%s", str_to_put);
		}
	}

	printList(head); // tester 
	free_list(head);
	free(str_to_put);
	fclose(new_fp);
	fclose(file_ptr);
	return new_file_name;
}

Macro* create_macro(char* name) {
	Macro* new_macro = (Macro*)malloc(sizeof(Macro));
	if (new_macro != NULL) {
		strcpy(new_macro->macro_name, name);
		strcpy(new_macro->macro_data, "");
		new_macro->next = NULL;
	}
	else
		error_handler("mamory allocation problem");

	return new_macro;
}

Macro* add_macro(Macro* head, Macro* macro_node) {

	macro_node->next = head;
	return macro_node;
}

char* get_macro(char* name, Macro* head) {

	Macro* temp_head = head;
	while (temp_head != NULL && (strcmp(name, temp_head->macro_name) != 0))
	{
		temp_head = temp_head->next;
	}

	if (!temp_head) {
		error_handler("Syntax error: ");
	}

	return temp_head->macro_data;
}


void free_list(Macro* head) {
	Macro* tmp;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

void printList(Macro* head) { // To test the list structs
	while (head != NULL)
	{
		printf("%s ", head->macro_name);
		printf("%s\n", head->macro_data);
		head = head->next;
	}
}