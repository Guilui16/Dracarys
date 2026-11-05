#ifndef RUBRO_NEGRA_H
#define RUBRO_NEGRA_H

#include "common.h"

typedef enum { VERMELHO, PRETO } Cor;

typedef struct NoRN {
    char chave[MAX_STR];
    Registro *registros;
    int qtd_registros;
    int cap_registros;
    Cor cor;
    struct NoRN *pai;
    struct NoRN *esquerda;
    struct NoRN *direita;
} NoRN;

typedef struct {
    NoRN *raiz;
    NoRN *NIL;
    int tamanho;
} ArvoreRubroNegra;

ArvoreRubroNegra* criar_rn();
void inserir_rn(ArvoreRubroNegra *arvore, const char *chave, Registro reg);
Registro* buscar_rn(ArvoreRubroNegra *arvore, const char *chave, int *qtd_encontrados);
int altura_rn(ArvoreRubroNegra *arvore);
int validar_rn(ArvoreRubroNegra *arvore);
void liberar_rn(ArvoreRubroNegra *arvore);

#endif
