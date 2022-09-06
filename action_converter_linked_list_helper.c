#include <stdio.h>
#include <stdlib.h>
#include "action_converter_linked_list_helper.h"




lablePtr add_lable_to_table(int line_counter, char* lable, lablePtr head) {
	lable[strlen(lable) - 1] = '\0';
	lablePtr newLable = create_new_lable(line_counter, lable);
	head = add_front(head, newLable);
	return head;
}

lablePtr add_front(lablePtr head, lablePtr newLable) {
	newLable->next = head;
	return newLable;
}

lablePtr create_new_lable(int line, char* lable_name) {
	lablePtr new_lable = (lablePtr)malloc(sizeof(lable));
	if (new_lable != NULL) {
		strcpy(new_lable->name, lable_name);
		new_lable->line = line;
		new_lable->next = NULL;
	}
	else {
		error_handler("There was an allocating problem");
	}
	return new_lable;
}

entryPtr add_entry_to_table(int line_counter, char* entry_lable, entryPtr head) {
	entryPtr new_entry_lable = create_new_entry(line_counter, entry_lable);
	head = add_front_entry(head, new_entry_lable);
	return head;
}

entryPtr add_front_entry(entryPtr head, entryPtr new_entry_lable) {
	new_entry_lable->next = head;
	return new_entry_lable;
}

entryPtr create_new_entry(int line, char* lable_name) {
	entryPtr new_entry_lable = (entryPtr)malloc(sizeof(entry));
	if (new_entry_lable != NULL) {
		strcpy(new_entry_lable->name, lable_name);
		new_entry_lable->line = line;
		new_entry_lable->next = NULL;
	}
	else {
		error_handler("There was an allocating problem");
	}
	return new_entry_lable;
}

externPtr add_extern_to_table(int line_counter, char* extern_lable, externPtr head) {
	externPtr new_extern_lable = create_new_extern(line_counter, extern_lable);
	head = add_front_extern(new_extern_lable, head);
	return head;
}

externPtr add_front_extern(externPtr new_extern_lable, externPtr head) {
	new_extern_lable->next = head;
	return new_extern_lable;
}

externPtr create_new_extern(int line, char* lable_name) {
	externPtr new_extern_lable = (externPtr)malloc(sizeof(external));
	if (new_extern_lable != NULL) {
		strcpy(new_extern_lable->name, lable_name);
		new_extern_lable->line = line;
		new_extern_lable->next = NULL;
	}
	else {
		error_handler("There was an allocating problem");
	}
	return new_extern_lable;
}