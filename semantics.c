#include "semantics.h"
#include <stdio.h>

table *tabela;

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

    aux = tabela->next_table;
    for (tmp = p->filhos->irmaos; tmp; tmp = tmp->irmaos)
    {
        if (strcmp(tmp->tipo, "MethodDecl") == 0)
        {
            errorcount += check_methodBody(tmp->filhos->irmaos, aux);
            if (aux->next_table == NULL)
            {
                break;
            }
            else
            {
                aux = aux->next_table;
            }
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
        if (strcmp(aux, "If") == 0)
        {
            // errorcount += check_if(p_aux, method_table);
        }
        if (strcmp(aux, "Assign") == 0)
        {
            // errorcount += check_assign(p_aux, method_table);
        }
        if (strcmp(aux, "Print") == 0)
        {
            errorcount += check_print(p_aux, method_table);
        }
        if (strcmp(aux, "Assign") == 0)
        {
            errorcount += check_assign(p_aux, method_table);
        }
    }

    return errorcount;
}

int check_assign(struct tnode *p, table *method_table)
{
    int errorcount = 0;

    expr_checks(p->filhos, method_table);
    if(p->filhos->data[0] == '\0'){
        printf("A data do filho é null\n");
        strcpy(p->data, "Undef");
    }
    else
        strcpy(p->data, p->filhos->data);

    return errorcount;
}

int check_print(struct tnode *p, table *method_table)
{

    char *type;
    if (strcmp(p->filhos->tipo, "StrLit") == 0){
        strcpy(p->filhos->data, "String");
    }

    if (strcmp(p->filhos->tipo, "Call") == 0)
    { // duas maneiras de fazer, dar check 1 a 1 ou, ter um check para expr que tem check para todos os nos dentro do expr
        check_call(p->filhos, method_table);
    }

    if (strcmp(p->filhos->tipo, "Add") == 0)
    {
        check_operations(p->filhos, method_table);
    }

    // Verificar se o tipo do print é correto no fim de tudo.
    if (!(strcmp(p->filhos->data, "int") == 0 | strcmp(p->filhos->data, "double") == 0 | strcmp(p->filhos->data, "boolean") == 0 | strcmp(p->filhos->data, "string") == 0))
    {
        printf("Erro no valor do print\n"); // depois mudar para o print correto
    }
}

int check_operations(struct tnode *p, table *method_table)
{
    int errorcount = 0;

    if (p->filhos->data[0] == '\0')
    {
        expr_checks(p->filhos, method_table);
    }
    
    if (strcmp(p->filhos->data, "int") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
           expr_checks(p->filhos->irmaos, method_table);

        if (strcmp(p->filhos->irmaos->data, "double") == 0)
        {
            strcpy(p->data, "double");
        }
        else if (strcmp(p->filhos->irmaos->data, "int") == 0)
        {
            strcpy(p->data, "int");
        }
        else
        {
            strcpy(p->data, "undef");
        }
    }

    else if (strcmp(p->filhos->data, "double") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table);

        if (strcmp(p->filhos->irmaos->data, "double") == 0 || strcmp(p->filhos->irmaos->data, "int") == 0)
        {
            strcpy(p->data, "double");
        }
        else
        {
            strcpy(p->data, "undef");
        }
    }

    return errorcount;
}

void expr_checks(struct tnode *p, table *method_table)
{
    struct tnode *node_aux;

    for (node_aux = p; node_aux; node_aux = node_aux->irmaos)
    {
        if (strcmp(node_aux->tipo, "Add") == 0)
        {   
            check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Sub") == 0)
        {
            // so por enquanto para nao dar erro.
           check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Mul") == 0)
        {
            // so por enquanto para nao dar erro.
           check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Div") == 0)
        {
            // so por enquanto para nao dar erro.
           check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Mod") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "And") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Or") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Xor") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Lshift") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Rshift") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Eq") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Ge") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Gt") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Le") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Lt") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Ne") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Minus") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Not") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp(node_aux->tipo, "Plus") == 0)
        {
            // so por enquanto para nao dar erro.
            strcpy(node_aux->data, "undef");
        }
        if (strcmp("DecLit", node_aux->tipo) == 0)
        {
            strcpy(node_aux->data, "int");
        }
        if (strcmp("RealLit", node_aux->tipo) == 0)
        {
            strcpy(node_aux->data, "double");
        }
        if (strcmp("BoolLit", node_aux->tipo) == 0)
        {
            strcpy(node_aux->data, "boolean");
        }
        if (strcmp("Id", node_aux->tipo) == 0)
        {

            table_element *aux = search_el(method_table, node_aux->valor);
            if (aux == NULL)
            {
                aux = search_el(tabela, node_aux->valor);
                if (aux == NULL)
                    strcpy(node_aux->data, "undef");
                else
                {
                    strcpy(node_aux->data, aux->type);
                }
            }
            else
            {
                strcpy(node_aux->data, aux->type);
            }
        }
    }
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

int check_if(struct tnode *p, table *method_table)
{
    int errorcount = 0;
    char *aux;
    strcpy(aux, p->filhos->tipo);
    if (strcmp(aux, "Eq") == 0 || strcmp(aux, "Ge") == 0 || strcmp(aux, "Gt") == 0 || strcmp(aux, "Le") == 0 || strcmp(aux, "Lt") == 0 || strcmp(aux, "Ne") == 0 || strcmp(aux, "Not") == 0 || strcmp(aux, "BoolLit") == 0)
    {
    }

    return errorcount;
}

int check_call(struct tnode *tnode, table *method_table)
{
    int errorcount = 0;
    int double_convert = 0;
    int counter = 0;
    table *aux;
    table *function = NULL;
    char arguments[40000];
    char aux_string[40000];
    table_element *el_aux;
    struct tnode *node_aux;

    table *test;
    memset(aux_string, 0, strlen(aux_string));
    create_argumets(tnode, method_table);

    for (test = tabela->next_table; test != NULL; test = test->next_table)
    {

        memset(arguments, 0, strlen(arguments));
        strcat(arguments, "(");

        table_element *params;
        params = test->table_element->next;

        if (tnode->filhos->irmaos != NULL)
        {
            for (node_aux = tnode->filhos->irmaos; node_aux; node_aux = node_aux->irmaos)
            {
                params = params->next;
                if (params == NULL)
                    break;

                if (params != NULL)
                {
                    if (strcmp(params->type, node_aux->data) != 0)
                    {
                        if (!(strcmp(node_aux->data, "int") == 0 && strcmp(params->type, "double") == 0))
                        {
                            // não é igual, vamos acabar este for e vamos para o proximo
                            break;
                        }
                    }
                }

                if ((strcmp(node_aux->data, "int") == 0 && strcmp(params->type, "double") == 0))
                    double_convert += 1;

                if (params->next != NULL)
                {
                    strcat(arguments, params->type);
                    strcat(arguments, ",");
                }
                else
                    strcat(arguments, params->type);

                if (params->next == NULL && node_aux->irmaos == NULL)
                {
                    if (function != NULL)
                    {
                        if (double_convert < counter)
                        {
                            function = test;
                            counter = double_convert;
                            strcpy(aux_string, arguments);
                        }
                    }
                    else
                    {
                        function = test;
                        counter = double_convert;
                        strcpy(aux_string, arguments);
                    }
                }
                double_convert = 0;
            }
        }
        else
        {

            if (params->next == NULL)
            {
                function = test;
                strcpy(aux_string, arguments);
            }
        }
    }
    strcat(aux_string, ")");
    if (function != NULL)
    {
        strcpy(tnode->data, function->tipo);
        strcpy(tnode->filhos->data, aux_string);
    }
    else
    {
        strcpy(tnode->data, "undef");
        strcpy(tnode->filhos->data, "undef");
    }

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

void create_argumets(struct tnode *tnode, table *method_table)
{

    struct tnode *node_aux;

    if (tnode->filhos->irmaos != NULL)
    {
        for (node_aux = tnode->filhos->irmaos; node_aux; node_aux = node_aux->irmaos)
        {
            if (strcmp("Call", node_aux->tipo) == 0)
            {
                check_call(node_aux, method_table);
            }
            else
            {
                expr_checks(node_aux, method_table);
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
