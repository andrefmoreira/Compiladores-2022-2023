#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <string.h>
#include <stdbool.h>
#include "tree.h"

typedef enum {integer, character, doub} basic_type;


typedef struct _t1{
	char name[65535];
	char type[65535];
	bool param;
	bool tabela;
	struct _t1 *next;
	bool valido;
} table_element;

typedef struct table{
	char nome[65535];
	char tipo[65535];
	bool valido;
	struct table* next_table;
	table_element* table_element;
} table;

table_element *insert_el(table_element* class_element, char *str, char *tipo, bool tabela, tnode* no);
void show_table();
table_element *search_el(table* symtab, char *str, bool tabela);
table *new_table(table* symtab, table* new_table);
bool check_new_table(table* symtab, table* new_table);

#endif
