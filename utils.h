#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void isFilePtrNullish(FILE* file_pt); //Exit if file pointer is null
void error_handler(char* str); // Prints the given error and exit the program
void write_error_to_file(FILE* file_pt, char* str);
char* concat(const char*, const char*);
char* replace_file_name_ending(char*, char*);
bool isLable(char*);

// TODO 5 - Add func to check if a verible name is forbidded (opcode/registers...) is_valid_name()
// TODO 5 - Add func to check if a number is valid should be integer is_valid_number()