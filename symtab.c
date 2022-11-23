
#include "symtab.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

table *new_table(table* symtab, table* new_table){
	table *aux;
	table* previous;
	new_table->next_table=NULL;
	if(symtab)	//Se table ja tem elementos
	{	//Procura cauda da lista e verifica se simbolo ja existe (NOTA: assume-se uma tabela de simbolos globais!)
		for(aux=symtab; aux; previous=aux, aux=aux->next_table)
			if(strcmp(aux->nome, new_table->nome)==0 && strcmp(aux->tipo, new_table->nome) == 0) //maybe good
				return NULL;
		
		previous->next_table=new_table;	//adiciona ao final da lista
	}
	else{	//symtab tem um elemento -> o novo simbolo
		symtab=new_table;
	}		
	return new_table; 
}

table_element *insert_el(table_element* symtab, char *str, char *tipo)
{
	table_element *newSymbol=(table_element*) malloc(sizeof(table_element));
	table_element *aux;
	table_element* previous;

	strcpy(newSymbol->name, str);
	strcpy(newSymbol->type, tipo);
	newSymbol->next=NULL;
	newSymbol->param=false;
	if(symtab)	//Se table ja tem elementos
	{	//Procura cauda da lista e verifica se simbolo ja existe (NOTA: assume-se uma tabela de simbolos globais!)
		for(aux=symtab; aux; previous=aux, aux=aux->next)
			if(strcmp(aux->name, str)==0 && strcmp(aux->type, str) == 0){ //maybe good
				return NULL;
			}
		previous->next=newSymbol;	//adiciona ao final da lista
	}
	else{	//symtab tem um elemento -> o novo simbolo
		symtab=newSymbol;
	}		
	return newSymbol; 
}

void show_table(table* symtab)
{
	printf("===== Class %s Symbol Table =====\n", symtab->nome);
	table* test;
	table_element* el_test;
	for(el_test = symtab->table_element->next; el_test; el_test = el_test->next){
		printf("%s\t\t%s\n",el_test->name,el_test->type);
	}
	for(test = symtab->next_table; test!=NULL; test = test->next_table){
		printf("%s\t(",test->nome);
		table_element* params;
		for(params = test->table_element->next; params!=NULL; params = params->next){
			if(params->param == true && params->next != NULL){
				if(params->next->param == true){
					printf("%s,",params->type);
				}
				else{
					printf("%s",params->type);
				}
			}
			else if(params->param == true && params->next == NULL){
				printf("%s",params->type);
			}
		}
		printf(")\t%s\n",test->tipo);
	}
	printf("\n");
	table *aux;
	table_element *el_aux;
	for(aux = symtab->next_table; aux; aux=aux->next_table){
		printf("===== Method %s(", aux->nome);
		for(el_aux = aux->table_element; el_aux; el_aux = el_aux->next){
			if(el_aux->param == true && el_aux->next != NULL){
				if(el_aux->next->param == true)
					printf("%s,",el_aux->type);
				else
					printf("%s",el_aux->type);
			}
			if(el_aux->param == true && el_aux->next == NULL)
				printf("%s",el_aux->type);
		}
		printf(") Symbol Table =====");
		for(el_aux = aux->table_element; el_aux; el_aux = el_aux->next){
			if(el_aux->param == true)
				printf("%s\t%s\t%s\n",el_aux->name, el_aux->type, "param");
			else
				printf("%s\t%s\n",el_aux->name, el_aux->type);
		}
		printf("\n");
	}
}
/*
//Procura um identificador, devolve 0 caso nao exista
table_element *search_el(char *str)
{
table_element *aux;

for(aux=symtab; aux; aux=aux->next)
	if(strcmp(aux->name, str)==0)
		return aux;

return NULL;
}
*/