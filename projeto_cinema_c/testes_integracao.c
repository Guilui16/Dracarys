#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "avl.h"
#include "rubro_negra.h"

int main() {
    ArvoreAVL *avl = criar_avl();
    ArvoreRubroNegra *rb = criar_rn();

    for (int i = 0; i < 16001; i++) {
        char chave[32];
        sprintf(chave, "filme %05d", i);
        Registro r;
        memset(&r, 0, sizeof(r));
        strcpy(r.tipo, "indicacao");
        inserir_avl(avl, chave, r);
    }

    for (int i = 0; i < 35000; i++) {
        char chave[32];
        sprintf(chave, "filme %05d", i);
        Registro r;
        memset(&r, 0, sizeof(r));
        strcpy(r.tipo, "filme");
        inserir_rn(rb, chave, r);
    }

    assert(validar_avl(avl) && validar_rn(rb));

    int cnt_a = 0, cnt_b = 0;
    Registro *a = buscar_avl(avl, "filme 00004", &cnt_a);
    Registro *b = buscar_rn(rb, "filme 00004", &cnt_b);

    assert(cnt_a == 1 && cnt_b == 1);
    assert(strcmp(a[0].tipo, "indicacao") == 0 && strcmp(b[0].tipo, "filme") == 0);
    assert(16001 + 35000 == 51001);

    printf("OK: 51.001 registros simulados com tipos separados; consulta complementar validada.\n");
    printf("AVL: 16.001 indicacoes simuladas | altura: %d\n", altura_avl(avl));
    printf("Rubro-negra: 35.000 filmes simulados | altura: %d\n", altura_rn(rb));

    liberar_avl(avl);
    liberar_rn(rb);
    return 0;
}
