#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils/index.h"
#include "./utils.h"
#include "./utils/static_data.h"

int main(int argc, char* argv[]) // argc = num of strings (file names), argv = the names array
{
	if (argc < 0) error_handler("\nno file name\n");
	int i;
	char* file_name_to_render;
	// Creates files (entry extern error...) for each file 
	for (i = 1; i < 2/*argc*/; i++) {
		char* file_name = strdup("jdhk.am"); //argv[i]
		if (!is_valid_file_name(file_name)) {
			char error[100] = "File name sould include .am. Current file name: ";
			strcat(error, file_name);
			error_handler(error);
		}
		file_name_to_render = create_spread_macro_file(file_name);
		action_converter(file_name_to_render); // Convert to 32 base and hande all the writing process
		free(file_name);
	}

	return 0;
}

