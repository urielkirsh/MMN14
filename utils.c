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

char* convert_bin_to_32b(char* bin) {
	if (strlen(bin) < 10) {
		printf("Not enough characters");
		exit(0);
	}
	char first_bin_str[5];
	char second_bin_str[5];
	int i = 0;
	int j = 0;
	for (i; i < 10; i++) {
		if (i > 4) {
			second_bin_str[j] = bin[i];
			j++;
		}
		first_bin_str[i] = bin[i];
	}
	// TODO 4 - Move to func or better solution

	int first_bin = convert_bin_to_dec(atoi(first_bin_str));
	int second_bin = convert_bin_to_dec(atoi(second_bin_str));
	char first_32base = BASE_NUMBERS[first_bin];
	char second_32base = BASE_NUMBERS[second_bin];
	char base32_num[2];
	base32_num[0] = first_32base;
	base32_num[1] = second_32base;

	return base32_num;
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
