
#include "symtab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

table *new_table(table *symtab, table *new_table)
{
	table *aux;
	table *previous;
	new_table->next_table = NULL;
	if (strcmp(new_table->tipo, "Bool") == 0)
		strcpy(new_table->tipo, "boolean");
	else if (strcmp(new_table->tipo, "Int") == 0)
		strcpy(new_table->tipo, "int");
	else if (strcmp(new_table->tipo, "Void") == 0)
		strcpy(new_table->tipo, "void");
	else if (strcmp(new_table->tipo, "Double") == 0)
		strcpy(new_table->tipo, "double");
	else if (strcmp(new_table->tipo, "StringArray") == 0)
		strcpy(new_table->tipo, "String[]");
	if (symtab) // Se table ja tem elementos
	{			// Procura cauda da lista e verifica se simbolo ja existe (NOTA: assume-se uma tabela de simbolos globais!)
		for (aux = symtab; aux; previous = aux, aux = aux->next_table)
			;
		previous->next_table = new_table; // adiciona ao final da lista
	}
	else
	{ // symtab tem um elemento -> o novo simbolo
		symtab = new_table;
	}
	table_element* el_aux = insert_el(symtab->table_element, new_table->nome, new_table->tipo, true, NULL);
	return new_table;
}

void check_new_table(table *symtab, table *new_table)
{
	table *aux;
	table_element *aux1, *aux2;
	new_table->valido = true;

	for (aux = symtab; aux; aux = aux->next_table)
	{
		if (strcmp(aux->nome, new_table->nome) == 0 && new_table != aux)
		{
			bool diferente = false;
			aux1 = aux->table_element->next;
			aux2 = new_table->table_element->next;
			if((aux1->next && !aux2->next) || (!aux1->next && aux2->next))
				return;
			for (; aux1 && aux2; aux1 = aux1->next, aux2 = aux2->next)
			{
				if (strcmp(aux1->type, aux2->type) != 0 && aux1->param && aux2->param)
				{
					diferente = true;
				}
			}
			if (!diferente)
			{
				// erro already defined
				new_table->valido = false;
				return;
			}
		}
	}
	return;
}

table_element *insert_el(table_element *symtab, char *str, char *tipo, bool tabela, tnode* no)
{
	table_element *newSymbol = (table_element *)malloc(sizeof(table_element));
	table_element *aux;
	table_element *previous;

	if (strcmp(tipo, "StringArray") == 0)
		strcpy(newSymbol->type, "String[]");
	else if (strcmp(tipo, "Int") == 0)
		strcpy(newSymbol->type, "int");
	else if (strcmp(tipo, "Bool") == 0)
		strcpy(newSymbol->type, "boolean");
	else if (strcmp(tipo, "Double") == 0)
		strcpy(newSymbol->type, "double");
	else if (strcmp(tipo, "Void") == 0)
		strcpy(newSymbol->type, "void");
	else
		strcpy(newSymbol->type, tipo);
	strcpy(newSymbol->name, str);
	newSymbol->next = NULL;
	newSymbol->param = false;
	newSymbol->valido = true;
	if(strcmp(newSymbol->name,"_")==0){
		newSymbol->valido = false;
	}
	if (symtab) // Se table ja tem elementos
	{			// Procura cauda da lista e verifica se simbolo ja existe (NOTA: assume-se uma tabela de simbolos globais!)
		for (aux = symtab; aux; previous = aux, aux = aux->next)
			if (strcmp(aux->name, str) == 0 && !tabela && !aux->tabela && aux->valido)
			{
				newSymbol->valido = false;
				printf("Line %d, col %d: Symbol %s already defined\n", no->linha, no->coluna, newSymbol->name);
			}
		previous->next = newSymbol; // adiciona ao final da lista
	}
	else
	{ // symtab tem um elemento -> o novo simbolo
		symtab = newSymbol;
	}
	newSymbol->tabela = tabela;
	return newSymbol;
}

void show_table(table *symtab)
{
	printf("===== Class %s Symbol Table =====\n", symtab->nome);
	table *test;
	table_element *el_test;
	table *tab_aux;
	if(symtab->next_table){
		tab_aux = symtab->next_table;
	}
	for (el_test = symtab->table_element->next; el_test; el_test = el_test->next)
	{
		if (!el_test->tabela && el_test->valido)
		{
			printf("%s\t\t%s\n", el_test->name, el_test->type);
		}
		else if(el_test->tabela && el_test->valido)
		{
			if(!tab_aux->valido){
				if(tab_aux->next_table){
					tab_aux = tab_aux->next_table;
				}
				continue;
			}
			printf("%s\t(", tab_aux->nome);
			table_element *params;
			for (params = tab_aux->table_element->next; params != NULL; params = params->next)
			{
				if (params->param == true && params->next != NULL)
				{
					if (params->next->param == true)
					{
						printf("%s,", params->type);
					}
					else
					{
						printf("%s", params->type);
					}
				}
				else if (params->param == true && params->next == NULL)
				{
					printf("%s", params->type);
				}
			}
			printf(")\t%s\n", tab_aux->tipo);
			if(tab_aux->next_table){
				tab_aux = tab_aux->next_table;
			}
		}
	}
	printf("\n");
	table *aux;
	table_element *el_aux;
	for (aux = symtab->next_table; aux; aux = aux->next_table)
	{
		if(!aux->valido){
			continue;
		}
		printf("===== Method %s(", aux->nome);
		for (el_aux = aux->table_element; el_aux; el_aux = el_aux->next)
		{
			if (el_aux->param == true && el_aux->next != NULL)
			{
				if (el_aux->next->param == true)
					printf("%s,", el_aux->type);
				else
					printf("%s", el_aux->type);
			}
			if (el_aux->param == true && el_aux->next == NULL)
				printf("%s", el_aux->type);
		}
		printf(") Symbol Table =====\n");
		for (el_aux = aux->table_element->next; el_aux; el_aux = el_aux->next)
		{
			if(!el_aux->valido){
				continue;
			}
			if (el_aux->param == true)
				printf("%s\t\t%s\t%s\n", el_aux->name, el_aux->type, "param");
			else
				printf("%s\t\t%s\n", el_aux->name, el_aux->type);
			
		}
		printf("\n");
	}
}

// Procura um identificador, devolve 0 caso nao exista
table_element *search_el(table *symtab, char *str, bool tabela)
{
	table_element *aux;
	if(!tabela){
		for (aux = symtab->table_element; aux; aux = aux->next)
			if (strcmp(aux->name, str) == 0 && !aux->tabela && aux->valido)
				return aux;
	}
	if(tabela){
		for (aux = symtab->table_element; aux; aux = aux->next)
			if (strcmp(aux->name, str) == 0 && aux->tabela && aux->valido)
				return aux;
	}
	return NULL;
}
