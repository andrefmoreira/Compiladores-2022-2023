#include <stdio.h>
#include <stdlib.h>
#define SIZE 1024

typedef struct list_node{
    tnode* node;
    lnode* next;
} lnode;


typedef struct tree_node{
    char* tipo;
    char* valor;
    lnode* irmaos;
    lnode* filhos;
} tnode;

typedef struct tree{
    tnode* raiz;
} tree;

tnode *add_node(char* nome, char* valor);

tnode *add_irmao(tnode* prIrmao, tnode* seIrmao);

tnode *add_filho(tnode* pai, tnode* filho);

void printTree(tnode* node, int nivel);