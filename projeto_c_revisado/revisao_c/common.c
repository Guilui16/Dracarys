#include "common.h"

void normalizar_string(const char *src, char *dest) {
    int j = 0;
    int space_flag = 0;
    if (!src) {
        dest[0] = '\0';
        return;
    }
    for (int i = 0; src[i] != '\0' && j < MAX_STR - 1; i++) {
        unsigned char c = (unsigned char)src[i];
        if (isalnum(c)) {
            dest[j++] = (char)tolower(c);
            space_flag = 0;
        } else {
            if (!space_flag && j > 0 && j < MAX_STR - 1) {
                dest[j++] = ' ';
                space_flag = 1;
            }
        }
    }
    if (j > 0 && dest[j - 1] == ' ') {
        j--;
    }
    dest[j] = '\0';
}

char** parse_csv_line(const char *line, int *col_count) {
    int capacity = 16;
    int count = 0;
    char **tokens = malloc(capacity * sizeof(char*));
    
    int len = strlen(line);
    char buf[MAX_LINE];
    int buf_idx = 0;
    int in_quotes = 0;

    for (int i = 0; i <= len; i++) {
        char c = line[i];
        if (c == '\r' || c == '\n') continue;

        if (c == '"') {
            in_quotes = !in_quotes;
        } else if ((c == ',' || c == '\0') && !in_quotes) {
            buf[buf_idx] = '\0';
            if (count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(char*));
            }
            tokens[count] = strdup(buf);
            count++;
            buf_idx = 0;
            if (c == '\0') break;
        } else {
            if (buf_idx < MAX_LINE - 1) {
                buf[buf_idx++] = c;
            }
        }
    }
    *col_count = count;
    return tokens;
}

void free_csv_line(char **tokens, int col_count) {
    if (!tokens) return;
    for (int i = 0; i < col_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
