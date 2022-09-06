
#include <stdio.h>

typedef struct lable_node* lablePtr;
typedef struct lable_node {
	int line;
	char name[10];
	lablePtr next;
} lable;

typedef struct entry_node* entryPtr;
typedef struct entry_node {
	int line;
	char name[10];
	entryPtr next;
} entry;

typedef struct extern_node* externPtr;
typedef struct extern_node {
	int line;
	char name[10];
	externPtr next;
} external;

lablePtr add_lable_to_table(int, char*, lablePtr);
lablePtr add_front(lablePtr, lablePtr);
lablePtr create_new_lable(int, char*);

entryPtr add_entry_to_table(int, char*, entryPtr);
entryPtr add_front_entry(entryPtr, entryPtr);
entryPtr create_new_entry(int, char*);

externPtr add_extern_to_table(int, char*, externPtr);
externPtr add_front_extern(externPtr, externPtr);
externPtr create_new_extern(int, char*);