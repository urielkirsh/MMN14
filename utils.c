#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./utils.h"



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

bool isLable(char* word) {
	if (word[strlen(word) - 1] == ':') {
		return true;
	}
	return false;
}