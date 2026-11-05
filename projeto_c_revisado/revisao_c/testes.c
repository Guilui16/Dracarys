#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "avl.h"
#include "rubro_negra.h"

void testar_avl(int n) {
    ArvoreAVL *avl = criar_avl();
    for (int i = 0; i < n; i++) {
        char chave[32];
        sprintf(chave, "%06d", i);
        Registro r;
        memset(&r, 0, sizeof(r));
        sprintf(r.filme, "Filme %d", i);
        inserir_avl(avl, chave, r);
    }
    assert(avl->tamanho == n);
    assert(validar_avl(avl));

    int cnt = 0;
    Registro *res = buscar_avl(avl, "000017", &cnt);
    assert(cnt == 1);
    assert(strcmp(res[0].filme, "Filme 17") == 0);

    Registro r_dup;
    memset(&r_dup, 0, sizeof(r_dup));
    strcpy(r_dup.filme, "Registro duplicado");
    inserir_avl(avl, "000017", r_dup);

    res = buscar_avl(avl, "000017", &cnt);
    assert(cnt == 2);
    assert(avl->tamanho == n);
    assert(validar_avl(avl));

    printf("ArvoreAVL OK, %d chaves, altura: %d\n", n, altura_avl(avl));
    liberar_avl(avl);
}

void testar_rn(int n) {
    ArvoreRubroNegra *rn = criar_rn();
    for (int i = 0; i < n; i++) {
        char chave[32];
        sprintf(chave, "%06d", i);
        Registro r;
        memset(&r, 0, sizeof(r));
        sprintf(r.filme, "Filme %d", i);
        inserir_rn(rn, chave, r);
    }
    assert(rn->tamanho == n);
    assert(validar_rn(rn));

    int cnt = 0;
    Registro *res = buscar_rn(rn, "000017", &cnt);
    assert(cnt == 1);
    assert(strcmp(res[0].filme, "Filme 17") == 0);

    Registro r_dup;
    memset(&r_dup, 0, sizeof(r_dup));
    strcpy(r_dup.filme, "Registro duplicado");
    inserir_rn(rn, "000017", r_dup);

    res = buscar_rn(rn, "000017", &cnt);
    assert(cnt == 2);
    assert(rn->tamanho == n);
    assert(validar_rn(rn));

    printf("ArvoreRubroNegra OK, %d chaves, altura: %d\n", n, altura_rn(rn));
    liberar_rn(rn);
}

int main() {
    testar_avl(50000);
    testar_rn(50000);
    return 0;
}
