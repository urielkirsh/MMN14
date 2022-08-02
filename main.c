#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils/index.h"

#define LINE_LENGTH 80

void write_binary_actions(char*, FILE*);

int main(int argc, char* argv[]) // argc = num of strings (file names), argv = the names array
{
	if (argc < 0) error_handeling("\nno file name\n");

	int i;
	char* file_name_to_render;
	for (i = 1; i < 2; i++) {/*argc*/
		// TODO 5 - Create array of pointers to the new files
		file_name_to_render = create_spread_macro_file("./file_test.txt"); // TODO 5 - How many files will be created here? /*argv[i]*/
		file_name_to_render = action_converter(file_name_to_render); // First run

	}

	return 0;
}

char* action_converter(char* file_name) {
	FILE* file_ptr = fopen(file_name,'a+');
	char* new_file_name = concat("binary_", file_name);
	FILE* new_fp = fopen(new_file_name, "wt"); // TODO 5 - Check for null
	char original_line[LINE_LENGTH];
	char copied_line[LINE_LENGTH];
	char* first_word;
	bool is_inside_of_bla = false;

	while (fgets(original_line, LINE_LENGTH, file_ptr) != NULL) {

		strcpy(copied_line, original_line);
		first_word = strtok(copied_line, " \n");
		if (copied_line[strlen(first_word) - 1] == ':') {
			is_inside_of_bla = !is_inside_of_bla;
			write_binary_actions(copied_line, new_fp);
		}
		if (is_inside_of_bla) {
			write_binary_actions(copied_line, new_fp);
		} else {

		}



	}

}

void write_binary_actions(char line[LINE_LENGTH], FILE* new_fp) {
	char* token = strtok(line, " ");
	if (token[strlen(token) - 1] == ':') {
		token = strtok(NULL, line);
	}
	char binary_word[10];

	while (token != NULL) {
		strcpy(binary_word ,get_binary_word(token));
		fprintf(new_fp, "%s", binary_word);
		token = strtok(NULL, line);
	}
}

char* get_binary_word(char* word) {

}