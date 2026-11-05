#ifndef AVL_H
#define AVL_H

#include "common.h"

typedef struct NoAVL {
    char chave[MAX_STR];
    Registro *registros;
    int qtd_registros;
    int cap_registros;
    struct NoAVL *esquerda;
    struct NoAVL *direita;
    int altura;
} NoAVL;

typedef struct {
    NoAVL *raiz;
    int tamanho;
} ArvoreAVL;

ArvoreAVL* criar_avl();
void inserir_avl(ArvoreAVL *arvore, const char *chave, Registro reg);
Registro* buscar_avl(ArvoreAVL *arvore, const char *chave, int *qtd_encontrados);
int altura_avl(ArvoreAVL *arvore);
int validar_avl(ArvoreAVL *arvore);
void liberar_avl(ArvoreAVL *arvore);

#endif
