#include "rubro_negra.h"

static void girar_esquerda(ArvoreRubroNegra *arvore, NoRN *x) {
    NoRN *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != arvore->NIL) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == arvore->NIL) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
}

static void girar_direita(ArvoreRubroNegra *arvore, NoRN *y) {
    NoRN *x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != arvore->NIL) {
        x->direita->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == arvore->NIL) {
        arvore->raiz = x;
    } else if (y == y->pai->direita) {
        y->pai->direita = x;
    } else {
        y->pai->esquerda = x;
    }
    x->direita = y;
    y->pai = x;
}

ArvoreRubroNegra* criar_rn() {
    ArvoreRubroNegra *arvore = malloc(sizeof(ArvoreRubroNegra));
    arvore->NIL = malloc(sizeof(NoRN));
    arvore->NIL->cor = PRETO;
    arvore->NIL->esquerda = arvore->NIL;
    arvore->NIL->direita = arvore->NIL;
    arvore->NIL->pai = arvore->NIL;
    arvore->raiz = arvore->NIL;
    arvore->tamanho = 0;
    return arvore;
}

static void corrigir_insercao(ArvoreRubroNegra *arvore, NoRN *z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            NoRN *tio = z->pai->pai->direita;
            if (tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    girar_esquerda(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                girar_direita(arvore, z->pai->pai);
            }
        } else {
            NoRN *tio = z->pai->pai->esquerda;
            if (tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    girar_direita(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                girar_esquerda(arvore, z->pai->pai);
            }
        }
    }
    arvore->raiz->cor = PRETO;
}

void inserir_rn(ArvoreRubroNegra *arvore, const char *chave, Registro reg) {
    NoRN *pai = arvore->NIL;
    NoRN *atual = arvore->raiz;

    while (atual != arvore->NIL) {
        pai = atual;
        int cmp = strcmp(chave, atual->chave);
        if (cmp == 0) {
            if (atual->qtd_registros >= atual->cap_registros) {
                atual->cap_registros *= 2;
                atual->registros = realloc(atual->registros, sizeof(Registro) * atual->cap_registros);
            }
            atual->registros[atual->qtd_registros++] = reg;
            return;
        }
        atual = (cmp < 0) ? atual->esquerda : atual->direita;
    }

    NoRN *novo = malloc(sizeof(NoRN));
    strncpy(novo->chave, chave, MAX_STR - 1);
    novo->chave[MAX_STR - 1] = '\0';
    novo->cap_registros = 4;
    novo->qtd_registros = 1;
    novo->registros = malloc(sizeof(Registro) * novo->cap_registros);
    novo->registros[0] = reg;
    novo->cor = VERMELHO;
    novo->esquerda = arvore->NIL;
    novo->direita = arvore->NIL;
    novo->pai = pai;

    if (pai == arvore->NIL) {
        arvore->raiz = novo;
    } else if (strcmp(chave, pai->chave) < 0) {
        pai->esquerda = novo;
    } else {
        pai->direita = novo;
    }

    arvore->tamanho++;
    corrigir_insercao(arvore, novo);
}

Registro* buscar_rn(ArvoreRubroNegra *arvore, const char *chave, int *qtd_encontrados) {
    *qtd_encontrados = 0;
    if (!arvore) return NULL;
    NoRN *atual = arvore->raiz;
    while (atual != arvore->NIL) {
        int cmp = strcmp(chave, atual->chave);
        if (cmp == 0) {
            *qtd_encontrados = atual->qtd_registros;
            return atual->registros;
        }
        atual = (cmp < 0) ? atual->esquerda : atual->direita;
    }
    return NULL;
}

static int altura_no_rn(ArvoreRubroNegra *arvore, NoRN *no) {
    if (no == arvore->NIL) return 0;
    int esq = altura_no_rn(arvore, no->esquerda);
    int dir = altura_no_rn(arvore, no->direita);
    return 1 + (esq > dir ? esq : dir);
}

int altura_rn(ArvoreRubroNegra *arvore) {
    return arvore ? altura_no_rn(arvore, arvore->raiz) : 0;
}

static int conferir_rn(ArvoreRubroNegra *arvore, NoRN *no) {
    if (no == arvore->NIL) return 1;
    if (no->cor == VERMELHO) {
        if (no->esquerda->cor == VERMELHO || no->direita->cor == VERMELHO) return -1;
    }
    int esq = conferir_rn(arvore, no->esquerda);
    int dir = conferir_rn(arvore, no->direita);
    if (esq == -1 || dir == -1 || esq != dir) return -1;
    return esq + (no->cor == PRETO ? 1 : 0);
}

int validar_rn(ArvoreRubroNegra *arvore) {
    if (!arvore) return 1;
    if (arvore->raiz->cor != PRETO || arvore->NIL->cor != PRETO) return 0;
    return conferir_rn(arvore, arvore->raiz) != -1;
}

static void liberar_nos_rn(ArvoreRubroNegra *arvore, NoRN *no) {
    if (no == arvore->NIL) return;
    liberar_nos_rn(arvore, no->esquerda);
    liberar_nos_rn(arvore, no->direita);
    free(no->registros);
    free(no);
}

void liberar_rn(ArvoreRubroNegra *arvore) {
    if (!arvore) return;
    liberar_nos_rn(arvore, arvore->raiz);
    free(arvore->NIL);
    free(arvore);
}
