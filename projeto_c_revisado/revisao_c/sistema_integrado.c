#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "avl.h"
#include "rubro_negra.h"

int ler_csv(const char *caminho, Registro **registros_out) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("Erro ao abrir arquivo %s\n", caminho);
        return 0;
    }
    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return 0;
    }

    int cap = 128;
    int total = 0;
    Registro *registros = malloc(sizeof(Registro) * cap);

    while (fgets(line, sizeof(line), f)) {
        int cols = 0;
        char **tokens = parse_csv_line(line, &cols);
        if (cols < 3) {
            free_csv_line(tokens, cols);
            continue;
        }

        if (total >= cap) {
            cap *= 2;
            registros = realloc(registros, sizeof(Registro) * cap);
        }

        memset(&registros[total], 0, sizeof(Registro));
        if (cols >= 12) { // formato indicacoes_reais
            strncpy(registros[total].id, tokens[0], MAX_STR - 1);
            strncpy(registros[total].premiacao, tokens[1], MAX_STR - 1);
            strncpy(registros[total].ano_cerimonia, tokens[2], MAX_STR - 1);
            strncpy(registros[total].ano_filme, tokens[3], MAX_STR - 1);
            strncpy(registros[total].categoria, tokens[4], MAX_STR - 1);
            strncpy(registros[total].indicado, tokens[5], MAX_STR - 1);
            strncpy(registros[total].filme, tokens[6], MAX_STR - 1);
            strncpy(registros[total].vencedor, tokens[7], MAX_STR - 1);
            strncpy(registros[total].genero, tokens[8], MAX_STR - 1);
            strncpy(registros[total].subgenero, tokens[9], MAX_STR - 1);
            strncpy(registros[total].imdb_id, tokens[10], MAX_STR - 1);
            strncpy(registros[total].fonte, tokens[11], MAX_STR - 1);
            if (cols >= 13) strncpy(registros[total].status_genero, tokens[12], MAX_STR - 1);
            strcpy(registros[total].tipo, "indicacao");
        } else { // formato catalogo_filmes
            strncpy(registros[total].imdb_id, tokens[0], MAX_STR - 1);
            strncpy(registros[total].filme, tokens[1], MAX_STR - 1);
            strncpy(registros[total].ano_filme, tokens[2], MAX_STR - 1);
            strncpy(registros[total].genero, tokens[3], MAX_STR - 1);
            if (cols >= 5) strncpy(registros[total].generos_imdb, tokens[4], MAX_STR - 1);
            if (cols >= 6) strncpy(registros[total].subgenero, tokens[5], MAX_STR - 1);
            if (cols >= 7) strncpy(registros[total].fonte, tokens[6], MAX_STR - 1);
            strcpy(registros[total].tipo, "filme");
        }

        total++;
        free_csv_line(tokens, cols);
    }

    fclose(f);
    *registros_out = registros;
    return total;
}

int main(int argc, char *argv[]) {
    const char *busca_titulo = "The Last Command";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--buscar") == 0 && i + 1 < argc) {
            busca_titulo = argv[i + 1];
        }
    }

    Registro *indicacoes = NULL;
    Registro *filmes = NULL;
    int qtd_ind = ler_csv("dados/indicacoes_reais.csv", &indicacoes);
    int qtd_filmes = ler_csv("dados/catalogo_filmes.csv", &filmes);

    ArvoreAVL *avl = criar_avl();
    ArvoreRubroNegra *rn = criar_rn();

    clock_t start = clock();
    for (int i = 0; i < qtd_ind; i++) {
        char chave[MAX_STR];
        const char *alvo = strlen(indicacoes[i].filme) > 0 ? indicacoes[i].filme : indicacoes[i].indicado;
        normalizar_string(alvo, chave);
        if (strlen(chave) == 0) sprintf(chave, "sem titulo %s", indicacoes[i].id);
        inserir_avl(avl, chave, indicacoes[i]);
    }
    double tempo_avl = (double)(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < qtd_filmes; i++) {
        char chave[MAX_STR];
        normalizar_string(filmes[i].filme, chave);
        inserir_rn(rn, chave, filmes[i]);
    }
    double tempo_rn = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("=== INDICES COMPLEMENTARES ===\n");
    printf("AVL: %d indicacoes | %d chaves | altura %d | montagem %.4fs\n", qtd_ind, avl->tamanho, altura_avl(avl), tempo_avl);
    printf("Rubro-negra: %d filmes | %d chaves | altura %d | montagem %.4fs\n", qtd_filmes, rn->tamanho, altura_rn(rn), tempo_rn);
    printf("TOTAL COMBINADO: %d linhas (tipos diferentes)\n", qtd_ind + qtd_filmes);

    char chave_busca[MAX_STR];
    normalizar_string(busca_titulo, chave_busca);

    start = clock();
    int res_ind_cnt = 0, res_film_cnt = 0;
    Registro *res_ind = buscar_avl(avl, chave_busca, &res_ind_cnt);
    Registro *res_film = buscar_rn(rn, chave_busca, &res_film_cnt);
    double tempo_busca = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("\nBusca por titulo: '%s' (%.8fs)\n", busca_titulo, tempo_busca);
    printf("Indicacoes: %d | Itens de catalogo: %d\n", res_ind_cnt, res_film_cnt);

    for (int i = 0; i < res_ind_cnt && i < 8; i++) {
        printf("  INDICACAO: %s %s %s vencedor=%s\n", res_ind[i].premiacao, res_ind[i].ano_cerimonia, res_ind[i].categoria, res_ind[i].vencedor);
    }
    for (int i = 0; i < res_film_cnt && i < 8; i++) {
        printf("  FILME: %s %s %s %s\n", res_film[i].filme, res_film[i].ano_filme, res_film[i].genero, res_film[i].imdb_id);
    }

    liberar_avl(avl);
    liberar_rn(rn);
    free(indicacoes);
    free(filmes);
    return 0;
}
