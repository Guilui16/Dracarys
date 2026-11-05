#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 2048
#define MAX_STR 256
#define MAX_RECORDS 128

typedef struct {
    char id[MAX_STR];
    char premiacao[MAX_STR];
    char ano_cerimonia[MAX_STR];
    char ano_filme[MAX_STR];
    char categoria[MAX_STR];
    char indicado[MAX_STR];
    char filme[MAX_STR];
    char vencedor[MAX_STR];
    char genero[MAX_STR];
    char generos_imdb[MAX_STR];
    char subgenero[MAX_STR];
    char imdb_id[MAX_STR];
    char fonte[MAX_STR];
    char status_genero[MAX_STR];
    char tipo[MAX_STR]; // "indicacao" ou "filme"
} Registro;

void normalizar_string(const char *src, char *dest);
char** parse_csv_line(const char *line, int *col_count);
void free_csv_line(char **tokens, int col_count);

#endif
