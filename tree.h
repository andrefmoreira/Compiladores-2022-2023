#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "struct.h"
#define SIZE 1024


typedef struct tnode{
    char* tipo;
    char* valor;
    struct tnode* irmaos;
    struct tnode* filhos;
    char data[40000];
    int linha;
    int coluna;
} tnode;

tnode *add_node(char* nome, new_struct* n);

tnode *add_irmao(tnode* prIrmao, tnode* seIrmao);

tnode *add_filho(tnode* pai, tnode* filho);

void printTree(tnode* node, int nivel);
#endif