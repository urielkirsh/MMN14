#include <stdio.h>
#include <stdlib.h>

typedef struct Macro {
	char macro_name[10];
	struct Macro* next;
	char macro_data[240];
} Macro;

char* create_spread_macro_file(char*);
char* spread_macro(FILE*, char*); // Returns a new file pointer where macroes embedded into the text

Macro* create_macro(char*); // Creates a new macro struct whitch contains name bytes and next pointer (to Null)
Macro* add_macro(Macro*, Macro*); // Add the given macro to the start as the head of the list and return it

char* get_macro(char*, Macro*); // Returns the macro as a string if exist (and exit if not found)

void printList(Macro*); // Prints the list for testing purposes
void free_list(Macro*);

int is_opcode(char*); // Return true (1) if there is opcode with that name