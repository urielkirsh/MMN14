#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "./utils.h"
#include "./utils/static_data.h"



void isFilePtrNullish(FILE* file_pt) {
	if (!file_pt) {
		error_handler("\nFile cannot be opened\n");
	}
}

void error_handler(char* str) {
	printf("%s\n", str);
	exit(0);
}

void write_error_to_file(char* error) {
	FILE* err_file_ptr = fopen("error_file", "a+");
	fprintf(err_file_ptr, "%s\n", error);
	fclose(err_file_ptr);
}

char* concat(const char* s1, const char* s2)
{
	char* result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
	if (result == NULL) { error_handler("Allocating problem"); }
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

char* replace_file_name_ending(char* file_name, char* new_end) {
	char* file_name_without_ending = strtok(file_name, ".");
	char* file_name_with_new_ending = concat(file_name_without_ending, new_end);
	char* new_file_name = concat(".", file_name_with_new_ending);
	return new_file_name;
}

bool is_lable(char* word) {
	if (word[strlen(word) - 1] == ':') {
		return true;
	}
	return false;
}

char* convert_dec_to_bin(int dec, bool negative) {
	
	// array to store binary number
	char binaryNum[8] = {'0','0','0','0','0','0','0','0'};
	int x = sizeof(binaryNum) / sizeof(binaryNum[0]);

	// counter for binary array
	int i = x-1;
	while (dec > 0) {
		// storing remainder in binary array
		binaryNum[i] = (dec % 2) + '0';
		dec = dec / 2;
		i--;
	}
	if (negative) {
		for (i = 0; i < 8; i++) {
			binaryNum[i] = (binaryNum[i] == '0') ? '1' : '0';
		}
	}
	char binStr[9];
	strncpy(binStr, binaryNum, x);
	binStr[x] = '\0';
	char* bin_str_to_return = binStr;
	return bin_str_to_return;
}
// Convert from dec to 32b
char* convert_dec_to_32b(char* bin, int base) {
	if (strlen(bin) < base) {
		printf("Not enough characters for 32b number");
		exit(0);
	}
	int string_length = 5;
	int string_cutter = 4;
	char* first_bin_str = (char*)malloc(sizeof(char) * (string_length+1));
	char* second_bin_str = (char*)malloc(sizeof(char) * (string_length+1));

	create_binary_string(first_bin_str, bin, 0, 0, 0, string_cutter); // Take the first part of the string (e.g "0000110000" -> "00001")
	create_binary_string(second_bin_str, bin, string_length, 0, 0, base); // Take the second part of the string (e.g "0000110000" -> "10000")

	int first_bin = convert_bin_to_dec(atoi(first_bin_str));
	int second_bin = convert_bin_to_dec(atoi(second_bin_str));
	char first_32base = BASE_NUMBERS[first_bin];
	char second_32base = BASE_NUMBERS[second_bin];
	char base32_num[2];
	base32_num[0] = first_32base;
	base32_num[1] = second_32base;
	free(first_bin_str);
	free(second_bin_str);
	return base32_num;
}

char* create_binary_string(char* str, char* bin, int i, int j, int flag, int len) {
	for (i; i <= len; i++) {
		if (bin[i] == '1' && flag == 0) {
			flag = 1;
		}
		if (flag == 1) {
			str[j] = bin[i];
			j++;
		}
	}
	str[len + 1] = '/0';
	return str;
}

int convert_bin_to_dec(int num) {
	int binary_num, decimal_num = 0, base = 1, rem;

	binary_num = num; // assign the binary number to the binary_num variable  
	while (num > 0)
	{
		rem = num % 10; /* divide the binary number by 10 and store the remainder in rem variable. */
		decimal_num = decimal_num + rem * base;
		num = num / 10; // divide the number with quotient  
		base = base * 2;
	}

	return decimal_num;
}

bool is_reg(char* word) {
	int i = 0;
	bool is_register = false;
	for (i; i < 9; i++) {
		if (strcmp(RGISTERS[i], word) == 0) {
			is_register = true;
		}
	}
	return is_register;
}

bool is_integer(double val)
{
	int truncated = (int)val;
	return (val == truncated);
}

int get_num_of_op_by_operation(char* op) {
	int i;
	int num_of_op = 0;
	for (i = 0; i < 9; i++) {
		if (strcmp(op, ONE_OPERANDS_ARR[i]) == 0) {
			num_of_op = 1;
		}
	}
	if (num_of_op == 0) {
		for (i = 0; i < 5; i++) {
			if (strcmp(op, TWO_OPERANDS_ARR[i]) == 0) {
				num_of_op = 2;
			}
		}
	}
	return num_of_op;
}

bool is_lable_name(char* name) {
	char first_letter = name[0];
	return (first_letter == toupper(first_letter));
}

bool is_opcode(char* name) {
	bool opcode = false;
	for (int i = 0; i < 16; i++) {
		if (strcmp(name, OPCODE[i]) == 0) {
			opcode = true;
		}
	}
	return opcode;
}

bool is_saved_words(char* verible_name) {
	bool saved_word = false;
	for (int i = 0; i < 5; i++) {
		if (strcmp(verible_name, SAVED_WORDS[i]) == 0) {
			saved_word = true;
		}
	}
	return saved_word;
}

bool is_valid_name(char* verible_name) {
	return (!is_reg(verible_name)) && (!is_opcode(verible_name)) && (!is_saved_words(verible_name));
}

bool is_valid_number(char* num) {
	int f = 0;
	for (int i = 0; num[i] != 0; i++) {
		if (num[i] == '.') {
			f = 1;
			break;
		}
	}

	if (f)
		return false;
	
	return true;
}

bool is_entry(char* first_word) {
	return (first_word == ".entry") ? true : false;
}

bool is_extern(char* first_word) {
	return (first_word == ".extern") ? true : false;
}

char* get_binary_reg(char* reg) {
	int i = 0;
	int dec_num;
	for (i = 0; i < 9; i++) {
		if (strcmp(reg, RGISTERS[i]) == 0) {
			dec_num = i;
		}
	}
	return BINARY_OPCODE[dec_num]; // Use the binary opcode cause its the same (4 digits)
}

