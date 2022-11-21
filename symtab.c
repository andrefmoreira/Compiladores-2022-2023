
#include "symtab.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

table_element *insert_el(table_element* symtab, char *str, char *tipo)
{
	table_element *newSymbol=(table_element*) malloc(sizeof(table_element));
	table_element *aux;
	table_element* previous;

	strcpy(newSymbol->name, str);
	strcpy(newSymbol->type, tipo);
	newSymbol->next=NULL;
	if(symtab)	//Se table ja tem elementos
	{	//Procura cauda da lista e verifica se simbolo ja existe (NOTA: assume-se uma tabela de simbolos globais!)
		for(aux=symtab; aux; previous=aux, aux=aux->next)
			if(strcmp(aux->name, str)==0)
				return NULL;
		
		previous->next=newSymbol;	//adiciona ao final da lista
	}
	else{	//symtab tem um elemento -> o novo simbolo
		symtab=newSymbol;
	}		
	
	return newSymbol; 
}

void show_table(table* symtab)
{
	printf("===== Class %s Symbol Table =====", symtab->nome);
	table_element* test;
	for(test = symtab->table_element; test; test= test->next){
		printf("%s\t%s\t\n",test->name,test->type);
	}
	table *aux;
	table_element *el_aux;
	for(aux = symtab->next_table; aux; aux=aux->next_table){
		printf("===== Method %s Symbol Table =====", aux->nome);
		for(el_aux = aux->table_element; el_aux; el_aux = el_aux->next){
			printf("%s\t%s\t\n",el_aux->name, el_aux->type);
		}
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