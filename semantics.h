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
void expr_checks(struct tnode* tnode, table *method_table, bool tabela);
int check_operations(struct tnode* tnode, table *method_table);
void error_expr(tnode* p);
int check_xor(struct tnode *p, table *method_table);
int check_shift(struct tnode *p, table *method_table);
int check_length(struct tnode *p, table *method_table);
int check_parseargs(struct tnode *p, table *method_table);
void check_relational(tnode* no , table *method_table);
void check_equality(tnode *no , table *method_table);
void check_and(tnode *no , table *method_table);
int check_double(struct tnode *p);
int check_int(struct tnode *p);
#endif