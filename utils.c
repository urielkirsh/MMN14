#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(suppress : 6387)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
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

void write_error_to_file(FILE* error_file_pt, char* error) {
	fprintf(error_file_pt, "%s\n", error);
}

char* concat(const char* s1, const char* s2)
{
	char* result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
	// in real code you would check for errors in malloc here
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

char* convert_dec_to_bin(int dec) {
	
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
	char binStr[10];
	memcpy(binStr, binaryNum, x);
	binStr[x] = '\0';
	printf("%s", binStr);
	return binStr;
}
// Convert from dec or hex to 32b (TODO 3 - Rename the name is misleading)
char* convert_bin_to_32b(char* bin, int base) {
	if (strlen(bin) < base) {
		printf("Not enough characters for 32b number");
		exit(0);
	}
	int string_length = (base == 8) ? 4 : 5;
	int string_cutter = (base == 8) ? 3 : 4;
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
	printf("%c", base32_num[0]);
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
