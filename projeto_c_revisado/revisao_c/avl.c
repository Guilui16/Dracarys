#include "avl.h"

static int max(int a, int b) { return (a > b) ? a : b; }

static int get_altura(NoAVL *n) { return n ? n->altura : 0; }

static void atualizar_altura(NoAVL *n) {
    if (n) n->altura = 1 + max(get_altura(n->esquerda), get_altura(n->direita));
}

static int get_balanceamento(NoAVL *n) {
    return n ? get_altura(n->esquerda) - get_altura(n->direita) : 0;
}

static NoAVL* girar_direita(NoAVL *y) {
    NoAVL *x = y->esquerda;
    NoAVL *T2 = x->direita;
    x->direita = y;
    y->esquerda = T2;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

static NoAVL* girar_esquerda(NoAVL *x) {
    NoAVL *y = x->direita;
    NoAVL *T2 = y->esquerda;
    y->esquerda = x;
    x->direita = T2;
    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}

ArvoreAVL* criar_avl() {
    ArvoreAVL *arvore = malloc(sizeof(ArvoreAVL));
    arvore->raiz = NULL;
    arvore->tamanho = 0;
    return arvore;
}

static NoAVL* criar_no_avl(const char *chave, Registro reg) {
    NoAVL *no = malloc(sizeof(NoAVL));
    strncpy(no->chave, chave, MAX_STR - 1);
    no->chave[MAX_STR - 1] = '\0';
    no->cap_registros = 4;
    no->qtd_registros = 1;
    no->registros = malloc(sizeof(Registro) * no->cap_registros);
    no->registros[0] = reg;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1;
    return no;
}

static NoAVL* inserir_no_avl(ArvoreAVL *arvore, NoAVL *no, const char *chave, Registro reg) {
    if (!no) {
        arvore->tamanho++;
        return criar_no_avl(chave, reg);
    }
    int cmp = strcmp(chave, no->chave);
    if (cmp == 0) {
        if (no->qtd_registros >= no->cap_registros) {
            no->cap_registros *= 2;
            no->registros = realloc(no->registros, sizeof(Registro) * no->cap_registros);
        }
        no->registros[no->qtd_registros++] = reg;
        return no;
    }
    if (cmp < 0) {
        no->esquerda = inserir_no_avl(arvore, no->esquerda, chave, reg);
    } else {
        no->direita = inserir_no_avl(arvore, no->direita, chave, reg);
    }

    atualizar_altura(no);
    int bal = get_balanceamento(no);

    if (bal > 1) {
        if (strcmp(chave, no->esquerda->chave) > 0) {
            no->esquerda = girar_esquerda(no->esquerda);
        }
        return girar_direita(no);
    }
    if (bal < -1) {
        if (strcmp(chave, no->direita->chave) < 0) {
            no->direita = girar_direita(no->direita);
        }
        return girar_esquerda(no);
    }
    return no;
}

void inserir_avl(ArvoreAVL *arvore, const char *chave, Registro reg) {
    if (arvore) {
        arvore->raiz = inserir_no_avl(arvore, arvore->raiz, chave, reg);
    }
}

Registro* buscar_avl(ArvoreAVL *arvore, const char *chave, int *qtd_encontrados) {
    *qtd_encontrados = 0;
    if (!arvore) return NULL;
    NoAVL *atual = arvore->raiz;
    while (atual) {
        int cmp = strcmp(chave, atual->chave);
        if (cmp == 0) {
            *qtd_encontrados = atual->qtd_registros;
            return atual->registros;
        }
        atual = (cmp < 0) ? atual->esquerda : atual->direita;
    }
    return NULL;
}

int altura_avl(ArvoreAVL *arvore) {
    return arvore ? get_altura(arvore->raiz) : 0;
}

static int conferir_avl(NoAVL *no) {
    if (!no) return 0;
    int esq = conferir_avl(no->esquerda);
    int dir = conferir_avl(no->direita);
    if (esq == -1 || dir == -1) return -1;
    int diff = esq - dir;
    if (diff < -1 || diff > 1) return -1;
    return max(esq, dir) + 1;
}

int validar_avl(ArvoreAVL *arvore) {
    if (!arvore || !arvore->raiz) return 1;
    return conferir_avl(arvore->raiz) != -1;
}

static void liberar_nos_avl(NoAVL *no) {
    if (!no) return;
    liberar_nos_avl(no->esquerda);
    liberar_nos_avl(no->direita);
    free(no->registros);
    free(no);
}

void liberar_avl(ArvoreAVL *arvore) {
    if (!arvore) return;
    liberar_nos_avl(arvore->raiz);
    free(arvore);
}
