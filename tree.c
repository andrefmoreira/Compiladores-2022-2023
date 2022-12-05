#include "tree.h"

tnode *add_node(char* nome, char* valor, int linha, int coluna){
    tnode* node = (tnode*) malloc(sizeof(tnode));
    node->tipo = nome;
    node->valor = valor;
    node->filhos = NULL;
    node->irmaos = NULL;
    node->linha = linha;
    node->coluna = coluna;
    
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
        //printf("%d,%d",node->linha,node->coluna);
        for (int i = 0; i < nivel; i++){
            printf("..");
        }
        if(node->valor == NULL){
            if(node->data[0] == '\0')
                printf("%s\n", node->tipo);
            else
                printf("%s - %s\n", node->tipo, node->data);
        } else {
            if(node->data[0] == '\0')
                printf("%s(%s)\n", node->tipo, node->valor);
            else
                 printf("%s(%s) - %s\n", node->tipo, node->valor , node->data);
        }
        printTree(node->filhos, (nivel+1));
        printTree(node->irmaos, nivel);

    }
}
