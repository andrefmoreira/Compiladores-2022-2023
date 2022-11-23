#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <stdlib.h>
#define SIZE 1024


typedef struct tnode{
    char* tipo;
    char* valor;
    struct tnode* irmaos;
    struct tnode* filhos;
    char* data;
} tnode;

tnode *add_node(char* nome, char* valor);

tnode *add_irmao(tnode* prIrmao, tnode* seIrmao);

tnode *add_filho(tnode* pai, tnode* filho);

void printTree(tnode* node, int nivel);
#endif