#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "symtab.h" 
#include "tree.h"
#include <stdbool.h>

int check_program(struct tnode* p);
int check_methodDecl(struct tnode* p);
int check_methodHeader(struct tnode* p, table* method_table);
int check_params(struct tnode* p, table* method_table);
int check_paramDecl(struct tnode* p, table* method_table);
int check_methodBody(struct tnode*p, table* method_table);
int check_var_decl(struct tnode* p, table* method_table);
int check_fieldDecl(struct tnode* p);
int check_call(struct tnode* tnode, table* method_table);
void create_argumets(struct tnode* tnode, table *method_table);
int check_if(struct tnode *p, table * method_table);
int check_assign(struct tnode*p, table* method_table);
int check_print(struct tnode *p, table *method_table);
#endif