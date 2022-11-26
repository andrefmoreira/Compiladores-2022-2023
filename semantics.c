#include "semantics.h"
#include <stdio.h>

table *tabela;
int second_phase;


int check_program(struct tnode *p)
{
    tabela = (table *)malloc(sizeof(table));
    table_element *new_el = (table_element *)malloc(sizeof(table_element));
    tabela->table_element = new_el;
    table *aux;
    int errorcount = 0;
    tnode *tmp;
    strcpy(tabela->nome, p->filhos->valor);
    for (tmp = p->filhos->irmaos; tmp; tmp = tmp->irmaos)
    {
        if (strcmp(tmp->tipo, "MethodDecl") == 0)
        {
            errorcount += check_methodDecl(tmp);
        }
        if (strcmp(tmp->tipo, "FieldDecl") == 0)
        {
            errorcount += check_fieldDecl(tmp);
        }
    }
    
    second_phase = 1;

    for (tmp = p->filhos->irmaos; tmp; tmp = tmp->irmaos)
    {
        if (strcmp(tmp->tipo, "MethodDecl") == 0)
        {
            errorcount += check_methodDecl(tmp);
        }
        if (strcmp(tmp->tipo, "FieldDecl") == 0)
        {
            errorcount += check_fieldDecl(tmp);
        }
    }



    return errorcount;
}

int check_fieldDecl(struct tnode *p)
{
    int errorcount = 0;
    table_element *new = insert_el(tabela->table_element, p->filhos->irmaos->valor, p->filhos->tipo);
    if (new == NULL)
    {
        // printf("Symbol %s already defined!\n", iid->id);
        return 1;
    }
    return errorcount;
}

int check_methodDecl(struct tnode *p)
{
    int errorcount = 0;
    table *method_table = (table *)malloc(sizeof(table));
    strcpy(method_table->nome, p->filhos->filhos->irmaos->valor);
    strcpy(method_table->tipo, p->filhos->filhos->tipo);
    table *new = new_table(tabela, method_table);
    if (new == NULL)
    {
        // printf("Symbol %s already defined!\n", iid->id);
        return 1;
    }

    errorcount += check_methodHeader(p->filhos, method_table);

    if(second_phase)
        errorcount += check_methodBody(p->filhos->irmaos, method_table);

    return errorcount;
}

int check_methodHeader(struct tnode *p, table *method_table)
{
    int errorcount = 0;
    table_element *aux = (table_element *)malloc(sizeof(table_element));
    method_table->table_element = aux;
    table_element *new_el = insert_el(method_table->table_element, "return", p->filhos->tipo);
    errorcount += check_params(p->filhos->irmaos->irmaos, method_table);
    return errorcount;
}

int check_params(struct tnode *p, table *method_table)
{
    int errorcount = 0;
    tnode *tmp;
    for (tmp = p->filhos; tmp; tmp = tmp->irmaos)
    {
        errorcount += check_paramDecl(tmp, method_table);

    }
    return errorcount;
}

int check_paramDecl(struct tnode *p, table *method_table)
{
    int errorcount = 0;
    table_element *new = insert_el(method_table->table_element, p->filhos->irmaos->valor, p->filhos->tipo);
    if (new == NULL)
    {
        // printf("Symbol %s already defined!\n", iid->id);
        return 1;
    }
    new->param = true;
    return errorcount;
}

int check_methodBody(struct tnode *p, table *method_table)
{
    int errorcount = 0;
    char aux[256];
    tnode *p_aux;
    table *aux1;

    for (p_aux = p->filhos; p_aux; p_aux = p_aux->irmaos)
    {
        strcpy(aux, p_aux->tipo);
        if (strcmp(aux, "VarDecl") == 0)
        {
            errorcount += check_var_decl(p_aux, method_table);


        }
        if (strcmp(aux, "Call") == 0)
        {
            errorcount += check_call(p_aux, method_table);
            
        }
    }
    return errorcount;
}

int check_var_decl(struct tnode *p, table *method_table)
{
    int errorcount = 0;
    table_element *new = insert_el(method_table->table_element, p->filhos->irmaos->valor, p->filhos->tipo);

    if (new == NULL)
    {
        // printf("Symbol %s already defined!\n", iid->id);
        return 1;
    }

    return errorcount;
}

int check_call(struct tnode *tnode, table *method_table)
{
    int errorcount = 0;
    table *aux;
    table_element *el_aux;
    struct tnode *node_aux;
    char arguments[40000];

    memset(arguments,0,strlen(arguments));

    create_argumets(tnode);
    strcat( arguments,"(");

    if (tnode->filhos->irmaos != NULL){
        for(node_aux = tnode->filhos->irmaos ; node_aux; node_aux = node_aux->irmaos){

            if(node_aux->data[0] != '\0'){
                if(node_aux->irmaos == NULL)
                    strcat(arguments , node_aux->data);
                else{
                    strcat(arguments , node_aux->data);
                    strcat(arguments , ",");
                }
            }

        }
    }
    strcat(arguments , ")");
    printf("ARGUMENTS NESTE MOMENTO %s\n" , arguments);
    strcpy(tnode->data , arguments);
    printf("olha o calll %s \n" , tnode->data);


    for (aux = tabela->next_table; aux; aux = aux->next_table)
    {

        if (strcmp(aux->nome, tnode->filhos->valor) == 0)
        {
            if (tnode->filhos->irmaos != NULL)
            { // tem argumentos

                if (aux->table_element->next != NULL)
                {
                    for (el_aux = aux->table_element->next, node_aux = tnode->filhos->irmaos; el_aux && node_aux; el_aux = el_aux->next, node_aux = node_aux->irmaos)
                    {
                        if (el_aux->param == true)
                        {
                            printf("%s,%s\n", el_aux->type, node_aux->tipo);
                            if (strcmp(el_aux->type, node_aux->valor) == 0)
                            {
                                printf("PASSOU\n");
                            }
                            else
                            {
                                printf("Argumento errado\n");
                                // Argumento errado
                                return 1;
                            }
                        }
                    }
                    if (el_aux)
                    {

                        if (el_aux->param == true)
                        {
                            // Falta argumentos
                            return 1;
                        }
                    }
                    if (node_aux)
                    {

                        // Argumentos a mais
                        return 1;
                    }
                }
                else
                {
                    printf("PASSOU1\n");
                    return 0;
                }
            }
            
        }
    }
    return errorcount;
}


void create_argumets(struct tnode* tnode){

struct tnode *node_aux;               

if (tnode->filhos->irmaos != NULL){
    for(node_aux = tnode->filhos->irmaos ; node_aux; node_aux = node_aux->irmaos){
        if(strcmp("DecLit" , node_aux->tipo) == 0){
            strcpy(node_aux->data , "int");
        }
        if(strcmp("RealLit" , node_aux->tipo) == 0){
            strcpy(node_aux->data , "double");
        }
        if(strcmp("BoolLit" , node_aux->tipo) == 0){
            strcpy(node_aux->data , "boolean");
        }
    }
}


}


/*
int check_vardec_list(is_vardec_list* ivl) {
    int errorcount=0;
    is_vardec_list* tmp;

    for(tmp=ivl; tmp; tmp=tmp->next)
        errorcount+=check_vardec(tmp->val);
    return errorcount;
}

int check_vardec(is_vardec* iv) {
    switch(iv->disc_d) {
        case d_integer:
            return check_integer_dec(iv->data_vardec.u_integer_dec);
        case d_character:
            return check_character_dec(iv->data_vardec.u_character_dec);
        case d_double:
            return check_double_dec(iv->data_vardec.u_double_dec);
    }
    return 0;
}

int check_integer_dec(is_integer_dec* iid) {
    table_element* newel=insert_el(iid->id, integer);

    if(newel==NULL) {
        printf("Symbol %s already defined!\n", iid->id);
        return 1;
    }
    return 0;
}

int check_character_dec(is_character_dec* icd) {
        table_element* newel=insert_el(icd->id, character);

        if(newel==NULL) {
            printf("Symbol %s already defined!\n", icd->id);
            return 1;
        }
        return 0;
}

int check_double_dec(is_double_dec* idd) {
    table_element* newel=insert_el(idd->id, doub);

    if(newel==NULL) {
        printf("Symbol %s already defined!\n", idd->id);
        return 1;
    }
    return 0;
}

int check_statement_list(is_statement_list* isl) {
    int errorcount=0;
    is_statement_list* tmp;

    for(tmp=isl; tmp; tmp=tmp->next)
        errorcount+=check_statement(tmp->val);
    return errorcount;
}


int check_statement(is_statement* is) {
    switch(is->disc_d) {
        case d_write: return check_write_statement(is->data_statement.u_write_statement);
    }
}

int check_write_statement(is_write_statement* iws) {
    table_element* aux=search_el(iws->id);

    if(aux==NULL) {
        printf("Symbol %s not declared!\n", iws->id);
        return 1;
    }
    return 0;
}

*/
