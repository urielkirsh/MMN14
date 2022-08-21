#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void isFilePtrNullish(FILE* file_pt); //Exit if file pointer is null
void error_handler(char* str); // Prints the given error and exit the program
void write_error_to_file(char* str);
char* concat(const char*, const char*);
char* replace_file_name_ending(char*, char*);
bool is_lable(char*); // Checks the last character for ':'
bool is_lable_name(char*); // Checks if the name starts with upper character

char* convert_dec_to_bin(int, bool); // Gets a decimal number and return string of binary number (base 8)
char* convert_dec_to_32b(char*, int); // Gets a decimal number and return string of 32base number (base 32)
int convert_bin_to_dec(int);// Gets a binary number and return int of decimal number (base 10)
char* create_binary_string(char*, char*, int, int, int, int);
bool is_reg(char*);
bool is_integer(double);
int get_num_of_op_by_operation(char*);

// TODO 5 - Add func to check if a verible name is forbidded (opcode/registers...) is_valid_name()
bool is_valid_name(char*);
// TODO 5 - Add func to check if a number is valid should be integer is_valid_number()
bool is_valid_number(char*);
bool is_opcode(char*);
bool is_saved_words(char*);
bool is_entry(char*);
bool is_extern(char*);

