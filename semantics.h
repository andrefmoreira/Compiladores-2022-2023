#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "symtab.h" 
#include "tree.h"
#include <stdbool.h>

int check_program(struct tnode* p);
int check_methodDecl(struct tnode* p, table_element* class_element);
int check_methodHeader(struct tnode* p, table_element* class_element, table* method_table);
int check_params(struct tnode* p, table* method_table);
int check_paramDecl(struct tnode* p, table_element* method_element);
int check_methodBody(struct tnode*p, table* method_table);
int check_var_decl(struct tnode* p, table* method_table);
#endif