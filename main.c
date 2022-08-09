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
	opPtr opcode_header = init_op_code_list();
	char* test = convert_bin_to_32b("0010010010");
	int i;
	char* file_name_to_render;
	for (i = 1; i < 2; i++) {/*argc*/
		// TODO 5 - Create array of pointers to the new files
		file_name_to_render = create_spread_macro_file("./file_test.txt"); // TODO 5 - How many files will be created here? /*argv[i]*/
		file_name_to_render = action_converter(file_name_to_render); // First run
	}

	return 0;
}

