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
    if(prIrmao != NULL && seIrmao != NULL){
        tnode * aux = prIrmao;
        for(aux = prIrmao; aux->irmaos; aux = aux->irmaos);
        aux->irmaos = seIrmao;

        return prIrmao;
    }
}

tnode *add_filho(tnode* pai, tnode* filho){
    if(pai != NULL && filho != NULL){
        pai->filhos = filho;
        return pai;
    }
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
        printTree(node->filhos, (nivel+1));
        printTree(node->irmaos, nivel);

    }
}
