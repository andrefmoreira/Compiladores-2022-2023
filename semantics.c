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
    table_element *new = insert_el(tabela->table_element, p->filhos->irmaos->valor, p->filhos->tipo, false, p->filhos->irmaos);
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
    errorcount += check_methodHeader(p->filhos, method_table);
    if (!check_new_table(tabela, method_table))
    {
        printf("Line %d, col %d: Symbol %s(", p->filhos->filhos->irmaos->linha, p->filhos->filhos->irmaos->coluna, p->filhos->filhos->irmaos->valor);
        table_element *aux;
        for (aux = method_table->table_element->next; aux; aux = aux->next)
        {
            if (aux->param == true && aux->next != NULL)
            {
                if (aux->next->param == true)
                    printf("%s,", aux->type);
                else
                    printf("%s", aux->type);
            }
            if (aux->param == true && aux->next == NULL)
                printf("%s", aux->type);
        }
        printf(") already defined\n");
    }
    table *new = new_table(tabela, method_table);
    return errorcount;
}

int check_methodHeader(struct tnode *p, table *method_table)
{
    int errorcount = 0;

    table_element *aux = (table_element *)malloc(sizeof(table_element));
    method_table->table_element = aux;
    table_element *new_el = insert_el(method_table->table_element, "return", p->filhos->tipo, false, NULL);
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
    table_element *new = insert_el(method_table->table_element, p->filhos->irmaos->valor, p->filhos->tipo, false, p->filhos->irmaos);
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
    if (!method_table->valido)
    {
        return 1;
    }
    if (p->data[0] != '\0')
        return 1;
    for (p_aux = p->filhos; p_aux; p_aux = p_aux->irmaos)
    {
        strcpy(aux, p_aux->tipo);
        if (strcmp(aux, "VarDecl") == 0)
        {
            errorcount += check_var_decl(p_aux, method_table);
        }
        else if (strcmp(aux, "Call") == 0)
        {
            if(p_aux->data[0] == '\0')
                errorcount += check_call(p_aux, method_table);
        }
        else if (strcmp(aux, "If") == 0)
        {
            expr_checks(p_aux->filhos, method_table, false);

            check_methodBody(p_aux, method_table);

            if (strcmp(p_aux->filhos->data, "boolean") != 0)
            {
                printf("Line %d, col %d: Incompatible type %s in if statement\n", p_aux->filhos->linha, p_aux->filhos->coluna, p_aux->filhos->data);
            }
        }
        else if (strcmp(aux, "While") == 0)
        {
            expr_checks(p_aux->filhos, method_table, false);

            check_methodBody(p_aux, method_table);

            if (strcmp(p_aux->filhos->data, "boolean") != 0)
            {
                printf("Line %d, col %d: Incompatible type %s in while statement\n", p_aux->filhos->linha, p_aux->filhos->coluna, p_aux->filhos->data);
            }
        }
        else if (strcmp(aux, "Block") == 0)
        {
            check_methodBody(p_aux , method_table);
        }
        else if (strcmp(aux, "Print") == 0)
        {
            if(p_aux->data[0] == '\0')
                errorcount += check_print(p_aux, method_table);
        }
        else if (strcmp(aux, "Assign") == 0)
        {
            if(p_aux->data[0] == '\0')
                errorcount += check_assign(p_aux, method_table);
        }
        else if (strcmp("ParseArgs", aux) == 0)
        {
            if(p_aux->data[0] == '\0')
                check_parseargs(p_aux, method_table);
        }
        else if (strcmp("Return", aux) == 0)
        {
            if (p_aux->filhos != NULL)
            {
                expr_checks(p_aux->filhos, method_table, false);
                if (strcmp(method_table->tipo, "void") == 0)
                {
                    printf("Line %d, col %d: Incompatible type %s in return statement\n", p_aux->linha, p_aux->filhos->coluna, p_aux->filhos->data);
                }
                else if (strcmp(method_table->tipo, p_aux->filhos->data) != 0)
                {
                    if (strcmp(method_table->tipo, "double") == 0)
                    {
                        if (strcmp(p_aux->filhos->data, "double") != 0)
                        {
                            if (strcmp(p_aux->filhos->data, "int") != 0)
                            {
                                printf("Line %d, col %d: Incompatible type %s in return statement\n", p_aux->linha, p_aux->filhos->coluna, p_aux->filhos->data);
                                continue;
                            }
                            else
                                continue;
                        }
                    }
                    printf("Line %d, col %d: Incompatible type %s in return statement\n", p_aux->linha, p_aux->filhos->coluna, p_aux->filhos->data);
                }
            }
            else
            {
                if (strcmp(method_table->tipo, "void") != 0)
                {
                    printf("Line %d, col %d: Incompatible type void in return statement\n", p_aux->linha, p_aux->coluna);
                }
            }
        }
    }

    return errorcount;
}

int check_print(struct tnode *p, table *method_table)
{

    char *type;
    if (strcmp(p->filhos->tipo, "StrLit") == 0)
    {
        strcpy(p->filhos->data, "String");
    }
    else if (strcmp(p->filhos->tipo, "Call") == 0)
    {
        check_call(p->filhos, method_table);
    }
    else if (strcmp(p->filhos->tipo, "Assign") == 0)
    {
        check_assign(p->filhos, method_table);
    }
    else
    {
        expr_checks(p->filhos, method_table, false);
    }

    // Verificar se o tipo do print é correto no fim de tudo.
    if ((strcmp(p->filhos->data, "int") == 0 || strcmp(p->filhos->data, "double") == 0 || strcmp(p->filhos->data, "boolean") == 0 || strcmp(p->filhos->data, "String") == 0))
    {
    }
    else
    {
        printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n", p->filhos->linha, p->filhos->coluna, p->filhos->data);
    }
}

int check_operations(struct tnode *p, table *method_table)
{
    int errorcount = 0;

    if (p->filhos->data[0] == '\0')
    {
        expr_checks(p->filhos, method_table, false);
    }

    if (strcmp(p->filhos->data, "int") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);

        else if (strcmp(p->filhos->irmaos->data, "double") == 0)
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
            error_expr(p);
        }
    }

    else if (strcmp(p->filhos->data, "double") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);

        else if (strcmp(p->filhos->irmaos->data, "double") == 0 || strcmp(p->filhos->irmaos->data, "int") == 0)
        {
            strcpy(p->data, "double");
        }
        else
        {
            strcpy(p->data, "undef");
            error_expr(p);
        }
    }
    else
    {
        strcpy(p->data, "undef");
        error_expr(p);
    }

    return errorcount;
}

void error_expr(tnode *p)
{
    if (strcmp(p->tipo, "Add") == 0)
    {
        printf("Line %d, col %d: Operator + cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
    if (strcmp(p->tipo, "Sub") == 0)
    {
        printf("Line %d, col %d: Operator - cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
    if (strcmp(p->tipo, "Mul") == 0)
    {
        printf("Line %d, col %d: Operator * cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
    if (strcmp(p->tipo, "Div") == 0)
    {
        printf("Line %d, col %d: Operator / cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
    if (strcmp(p->tipo, "Mod") == 0)
    {
        printf("Line %d, col %d: Operator %% cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
}

int check_value(struct tnode *p, table *method_table)
{

    if (p->filhos->data[0] == '\0')
    {
        expr_checks(p->filhos, method_table, false);
    }

    if (strcmp(p->tipo, "Not") == 0)
    {
        if (strcmp(p->filhos->data, "boolean") == 0)
        {
            strcpy(p->data, p->filhos->data);
        }
        else
        {
            strcpy(p->data, "boolean");
            printf("Line %d, col %d: Operator ! cannot be applied to type %s\n", p->linha, p->coluna, p->filhos->data);
        }
    }
    else
    {
        if (strcmp(p->filhos->data, "int") == 0 || strcmp(p->filhos->data, "double") == 0)
        {
            strcpy(p->data, p->filhos->data);
        }
        else
        {
            strcpy(p->data, "undef");
            printf("Line %d, col %d: Operator + cannot be applied to type %s\n", p->linha, p->coluna, p->filhos->data);
        }
    }
}

void expr_checks(struct tnode *p, table *method_table, bool is_table)
{
    struct tnode *node_aux;

    for (node_aux = p; node_aux; node_aux = node_aux->irmaos)
    {
        if (strcmp(node_aux->tipo, "Call") == 0)
        {
            check_call(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Add") == 0)
        {
            check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Sub") == 0)
        {
            check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Mul") == 0)
        {
            check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Div") == 0)
        {
            check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Mod") == 0)
        {
            check_operations(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "And") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_and(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Or") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_and(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Xor") == 0)
        {
            check_xor(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Lshift") == 0)
        {
            check_shift(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Rshift") == 0)
        {
            check_shift(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Eq") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_equality(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Ge") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_relational(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Gt") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_relational(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Le") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_relational(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Lt") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_relational(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Ne") == 0)
        {
            strcpy(node_aux->data, "boolean");
            if (node_aux->filhos->data[0] == '\0')
                expr_checks(node_aux->filhos, method_table, false);
            check_equality(node_aux,method_table);
        }
        if (strcmp(node_aux->tipo, "Minus") == 0)
        {
            check_value(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Not") == 0)
        {
            check_value(node_aux, method_table);
        }
        if (strcmp(node_aux->tipo, "Plus") == 0)
        {
            check_value(node_aux, method_table);
        }
        if (strcmp("DecLit", node_aux->tipo) == 0)
        {
            strcpy(node_aux->data, "int");
            check_int(node_aux);
        }
        if (strcmp("RealLit", node_aux->tipo) == 0)
        {
            strcpy(node_aux->data, "double");
            check_double(node_aux);
        }
        if (strcmp("BoolLit", node_aux->tipo) == 0)
        {
            strcpy(node_aux->data, "boolean");
        }
        if (strcmp("Assign", node_aux->tipo) == 0)
        {
            check_assign(node_aux, method_table);
        }
        if (strcmp("Length", node_aux->tipo) == 0)
        {
            check_length(node_aux, method_table);
        }
        if (strcmp("ParseArgs", node_aux->tipo) == 0)
        {
            check_parseargs(node_aux, method_table);
        }
        if (strcmp("Id", node_aux->tipo) == 0)
        {
            table_element *aux = search_el(method_table, node_aux->valor, is_table);
            if (aux == NULL)
            {
                aux = search_el(tabela, node_aux->valor, is_table);
                if (aux == NULL)
                {
                    if (!is_table)
                        printf("Line %d, col %d: Cannot find symbol %s\n", node_aux->linha, node_aux->coluna, node_aux->valor);
                    strcpy(node_aux->data, "undef");
                }
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

int check_int(struct tnode *p)
{

    int length = strlen(p->valor);
    char valor[100];
    memset(valor, 0, strlen(valor));
    int y = 0;

    for (int i = 0; i < length; i++)
    {
        if (p->valor[i] >= '0' && p->valor[i] <= '9')
        {
            valor[y] = p->valor[i];
            y++;
        }
    }

    if (strlen(valor) > 10)
    {
        printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
    }
    else if (strlen(valor) == 10)
    {
        if (valor[9] > '8')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[8] > '4')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[7] > '6')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[6] > '3')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[5] > '8')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[4] > '4')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[3] > '7')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[2] > '4')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[1] > '1')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (valor[0] > '2')
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
        else if (strcmp(valor, "2147483648") == 0)
            printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
    }
}

int check_double(struct tnode *p)
{

    char valor_aux[100];
    int y = 0;
    valor_aux[0] = '\0';
    int option = 0;

    for (int i = 0; p->valor[i] != '\0'; i++)
    {
        if (p->valor[i] == 'E')
        {
            option = 1;
        }
    }

    char *str = strdup(p->valor);
    char *valor = NULL;

    if (option)
        valor = strtok(str, "E");
    else
        valor = strtok(str, "e");

    for (int i = 0; i < strlen(p->valor); i++)
    {
        if (p->valor[i] != '_')
        {
            valor_aux[y] = p->valor[i];
            y++;
        }
    }
    valor_aux[y] = '\0';
    if (atof(valor_aux) == 0 && atof(valor) != 0)
        printf("Line %d, col %d: Number %s out of bounds\n", p->linha, p->coluna, p->valor);
}

void check_and(tnode *no, table *method_table)
{
    char *aux;
    if (strcmp(no->tipo, "And") == 0)
        aux = strdup("&&");
    if (strcmp(no->tipo, "Or") == 0)
        aux = strdup("||");

    if (no->filhos->data[0] == '\0')
    {
        expr_checks(no->filhos, method_table, false);
    }
    if (no->filhos->irmaos->data[0] == '\0')
    {
        expr_checks(no->filhos->irmaos, method_table, false);
    } 
    if (strcmp(no->filhos->data, no->filhos->irmaos->data) != 0)
    {
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", no->linha, no->coluna, aux, no->filhos->data, no->filhos->irmaos->data);
    }
    else
    {
        if (strcmp(no->filhos->data, "boolean") != 0)
        {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", no->linha, no->coluna, aux, no->filhos->data, no->filhos->irmaos->data);
        }
    }
}

void check_equality(tnode *no, table *method_table)
{
    char *aux;
    if (strcmp(no->tipo, "Eq") == 0)
        aux = strdup("==");
    if (strcmp(no->tipo, "Ne") == 0)
        aux = strdup("!=");

    if (no->filhos->data[0] == '\0')
    {
        expr_checks(no->filhos, method_table, false);
    }
    if (no->filhos->irmaos->data[0] == '\0')
    {
        expr_checks(no->filhos->irmaos, method_table, false);
    }        
    if (strcmp(no->filhos->data, no->filhos->irmaos->data) != 0)
    {
        if ((strcmp(no->filhos->data, "double") == 0 && strcmp(no->filhos->irmaos->data, "int") == 0) || (strcmp(no->filhos->data, "int") == 0 && strcmp(no->filhos->irmaos->data, "double") == 0))
            return;
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", no->linha, no->coluna, aux, no->filhos->data, no->filhos->irmaos->data);
    }
    else
    {
        if (strcmp(no->filhos->data, "String[]") == 0 || strcmp(no->filhos->data , "undef") == 0 || strcmp(no->filhos->data, "String") == 0)
        {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", no->linha, no->coluna, aux, no->filhos->data, no->filhos->irmaos->data);
        }
    }
}

void check_relational(tnode *no, table *method_table)
{
    char *aux;
    if (strcmp(no->tipo, "Ge") == 0)
        aux = strdup(">=");
    if (strcmp(no->tipo, "Gt") == 0)
        aux = strdup(">");
    if (strcmp(no->tipo, "Le") == 0)
        aux = strdup("<=");
    if (strcmp(no->tipo, "Lt") == 0)
        aux = strdup("<");
    if (no->filhos->data[0] == '\0')
    {
        expr_checks(no->filhos, method_table, false);
    }
    if (no->filhos->irmaos->data[0] == '\0')
    {
        expr_checks(no->filhos->irmaos, method_table, false);
    }
    if (strcmp(no->filhos->data, no->filhos->irmaos->data) != 0)
    {
        if ((strcmp(no->filhos->data, "double") == 0 && strcmp(no->filhos->irmaos->data, "int") == 0) || (strcmp(no->filhos->data, "int") == 0 && strcmp(no->filhos->irmaos->data, "double") == 0))
            return;
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", no->linha, no->coluna, aux, no->filhos->data, no->filhos->irmaos->data);
    }
    else
    {
        if (strcmp(no->filhos->data, "String[]") == 0 || strcmp(no->filhos->data, "boolean") == 0 || strcmp(no->filhos->data, "undef") == 0)
        {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", no->linha, no->coluna, aux, no->filhos->data, no->filhos->irmaos->data);
        }
    }
}

int check_parseargs(struct tnode *p, table *method_table)
{
    strcpy(p->data, "int");
    if (p->filhos->data[0] == '\0')
    {
        expr_checks(p->filhos, method_table, false);
    }
    if (strcmp(p->filhos->data, "String[]") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);

        if (strcmp(p->filhos->irmaos->data, "int") != 0)
        {
            printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
        }
    }
    else
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);

        printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
}

int check_length(struct tnode *p, table *method_table)
{
    strcpy(p->data, "int");

    if (p->filhos->data[0] == '\0')
    {
        expr_checks(p->filhos, method_table, false);
    }

    if (strcmp(p->filhos->data, "String[]") != 0)
    {
        printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", p->linha, p->coluna, p->filhos->data);
    }
}

int check_xor(struct tnode *p, table *method_table)
{
    if (p->filhos->data[0] == '\0')
    {
        expr_checks(p->filhos, method_table, false);
    }

    if (strcmp(p->filhos->data, "int") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);

        if (strcmp(p->filhos->irmaos->data, "int") == 0)
        {
            strcpy(p->data, "int");
        }
        else
        {
            strcpy(p->data, "undef");
            printf("Line %d, col %d: Operator ^ cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
        }
    }

    else if (strcmp(p->filhos->data, "boolean") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);

        if (strcmp(p->filhos->irmaos->data, "boolean") == 0)
        {
            strcpy(p->data, "boolean");
        }
        else
        {
            strcpy(p->data, "undef");
            printf("Line %d, col %d: Operator ^ cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
        }
    }

    else
    {
        strcpy(p->data, "undef");
        printf("Line %d, col %d: Operator ^ cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
}

int check_shift(struct tnode *p, table *method_table)
{

    if (p->filhos->data[0] == '\0')
    {
        expr_checks(p->filhos, method_table, false);
    }

    if (strcmp(p->filhos->data, "int") == 0)
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);

        if (strcmp(p->filhos->irmaos->data, "int") == 0)
        {
            strcpy(p->data, "int");
        }

        else
        {
            strcpy(p->data, "undef");
            if (strcmp(p->tipo, "Lshift") == 0)
                printf("Line %d, col %d: Operator << cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
            if (strcmp(p->tipo, "Rshift") == 0)
                printf("Line %d, col %d: Operator >> cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
        }
    }
    else
    {
        if (p->filhos->irmaos->data[0] == '\0')
            expr_checks(p->filhos->irmaos, method_table, false);        
        strcpy(p->data, "undef");
        if (strcmp(p->tipo, "Lshift") == 0)
            printf("Line %d, col %d: Operator << cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
        if (strcmp(p->tipo, "Rshift") == 0)
            printf("Line %d, col %d: Operator >> cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
}

int check_assign(struct tnode *p, table *method_table)
{
    int errorcount = 0;

    if(p->filhos->data[0] == '\0')
        expr_checks(p->filhos, method_table, false);
    strcpy(p->data, p->filhos->data);
    if (p->filhos->irmaos->data[0] == '\0')
        expr_checks(p->filhos->irmaos, method_table, false);

    if (strcmp(p->filhos->data, "String[]") == 0 && strcmp(p->filhos->irmaos->data, "String[]") == 0)
    {
        printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", p->linha, p->coluna, p->filhos->data, p->filhos->irmaos->data);
    }
    if (strcmp(p->filhos->data, "double") == 0)
    {
        if (strcmp(p->filhos->irmaos->data, "double") == 0 || strcmp(p->filhos->irmaos->data, "int") == 0)
        {
        }
        else
        {
            printf("Line %d, col %d: Operator = cannot be applied to types double, %s\n", p->linha, p->coluna, p->filhos->irmaos->data);
        }
    }
    else if (strcmp(p->filhos->data, p->filhos->irmaos->data) != 0 || strcmp(p->filhos->data , "undef") == 0 )
    {
        printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", p->linha, p->coluna, p->data, p->filhos->irmaos->data);
    }
    return errorcount;
}

int check_var_decl(struct tnode *p, table *method_table)
{
    int errorcount = 0;

    table_element *new = insert_el(method_table->table_element, p->filhos->irmaos->valor, p->filhos->tipo, false, p->filhos->irmaos);
    if (new == NULL)
    {
        // printf("Symbol %s already defined!\n", iid->id);
        return 1;
    }

    return errorcount;
}

int check_call(struct tnode *tnode, table *method_table)
{
    if(tnode->data[0]!='\0')
        return 1 ;

    int errorcount = 0;
    int double_convert = 0;
    int counter = 0;
    int ambiguidade = 0;
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
        double_convert = 0;

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
                {
                    double_convert += 1;
                }

                if (params->next != NULL && params->next->param)
                {

                    strcat(arguments, params->type);
                    strcat(arguments, ",");
                }
                else
                {
                    strcat(arguments, params->type);
                }

                if ((params->next == NULL || params->next->param == false) && node_aux->irmaos == NULL)
                {
                    if (function != NULL)
                    {
                        if (counter != 0 && (strcmp(test->nome, tnode->filhos->valor) == 0))
                        {
                            if (double_convert != 0)
                            {
                                ambiguidade++;
                            }
                            else
                            {
                                if (strcmp(test->nome, tnode->filhos->valor) == 0)
                                {
                                    function = test;
                                    counter = double_convert;
                                    strcpy(aux_string, arguments);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (strcmp(test->nome, tnode->filhos->valor) == 0)
                        {
                            function = test;
                            counter = double_convert;
                            strcpy(aux_string, arguments);
                        }
                    }
                }
            }
        }
        else
        {
            if (params->next == NULL)
            {
                if (strcmp(test->nome, tnode->filhos->valor) == 0)
                {
                    function = test;
                    strcpy(aux_string, arguments);
                }
            }
        }
    }

    strcat(aux_string, ")");

    if (function != NULL)
    {
        if (ambiguidade > 0 && counter != 0)
        {
            strcpy(tnode->data, "undef");
            strcpy(tnode->filhos->data, "undef");
            printf("Line %d, col %d: Reference to method %s(", tnode->linha, tnode->coluna, tnode->filhos->valor);
            if (tnode->filhos->irmaos)
            {
                struct tnode *t_aux;
                for (t_aux = tnode->filhos->irmaos; t_aux; t_aux = t_aux->irmaos)
                {
                    if (!t_aux->irmaos)
                        printf("%s", t_aux->data);
                    else
                        printf("%s,", t_aux->data);
                }
            }
            printf(") is ambiguous\n");
        }
        else
        {
            strcpy(tnode->data, function->tipo);
            strcpy(tnode->filhos->data, aux_string);
        }
    }
    else
    {
        strcpy(tnode->data, "undef");
        strcpy(tnode->filhos->data, "undef");
        printf("Line %d, col %d: Cannot find symbol %s(", tnode->linha, tnode->coluna, tnode->filhos->valor);
        if (tnode->filhos->irmaos)
        {
            struct tnode *t_aux;
            for (t_aux = tnode->filhos->irmaos; t_aux; t_aux = t_aux->irmaos)
            {
                if (!t_aux->irmaos)
                    printf("%s", t_aux->data);
                else
                    printf("%s,", t_aux->data);
            }
        }
        printf(")\n");
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
                            // printf("%s,%s\n", el_aux->type, node_aux->tipo);
                            if (strcmp(el_aux->type, node_aux->valor) == 0)
                            {
                            }
                            else
                            {
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
                expr_checks(node_aux, method_table, false);
            }
        }
    }
}
