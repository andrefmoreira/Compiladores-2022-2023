#include "tree.h"

tnode *add_node(char* nome, char* valor){
    tnode* node = (tnode*) malloc(sizeof(tnode));
    node->tipo = nome;
    node->valor = valor;
    node->filhos = NULL;
    node->irmaos = NULL;
    
    return node;
}

tnode *add_irmao(tnode* prIrmao, tnode* seIrmao){
    lnode* aux = prIrmao->irmaos;
    if(aux != NULL){
        for(; aux->next != NULL; aux = aux->next);
    }
    else{
        lnode* irmaos = (lnode*) malloc(sizeof(lnode));
        aux = irmaos;
        
    }
    aux->node = seIrmao;
    return prIrmao;
}

tnode *add_filho(tnode* pai, tnode* filho){
    lnode* aux = pai->filhos;
    if(aux != NULL){
        for(;aux->next != NULL; aux = aux->next);
    }
    else{
        lnode* filhos = (lnode*) malloc(sizeof(lnode));
        aux = filhos;
    }
    aux->node = filho;
    return pai;
}

void printTree(tnode* node, int nivel){
    if(node != NULL){

        for (int i = 0; i < nivel; i++){
            printf("..");
        }
        if(node->valor == NULL){
            printf("%s\n", node->tipo);
        } else {
            printf("%s(%s)\n", node->tipo, node->valor);
        }
        printTree(node->filhos->node, (nivel+1));
        printTree(node->irmaos->node, nivel);

    }
}
