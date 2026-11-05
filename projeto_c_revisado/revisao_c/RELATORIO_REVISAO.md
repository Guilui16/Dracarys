# Revisao da versao em C

## Verificacoes executadas

- Compilacao com GCC 14.2, `-Wall -Wextra -O2`: aprovada apos correcao de constante.
- `./testes`: AVL e rubro-negra com 50.000 chaves distintas cada: aprovados.
- `./testes_integracao`: 16.001 + 35.000 registros simulados: aprovado.
- Consultas reais sobre os CSVs anexados: The Last Command (2 indicacoes e 1 filme) e Gone with the Wind (1 indicacao e 1 filme): aprovadas.
- AddressSanitizer + UndefinedBehaviorSanitizer: consulta com CSVs de amostra aprovada, sem avisos.

## Correciones realizadas nesta copia

1. `CLOCKSPERSEC` substituido por `CLOCKS_PER_SEC` em `sistema_integrado.c`.
2. `normalizar_string` passou a respeitar `MAX_STR - 1` e evitar escrita fora do buffer.
3. `conferir_avl` passou a propagar corretamente retorno de subarvore invalida (`-1`).
4. Leitura do CSV de indicacoes agora preserva `status_genero` quando presente.

## Limites e pendencias

- CSVs enviados possuem 5 indicacoes e 3 filmes, apenas 8 linhas de dados. A meta de mais de 50 mil registros historicos REAIS nao foi validada.
- Nao ha remocao nem percursos publicos em ordem/pre-ordem/pos-ordem.
- O parser CSV simples nao cobre todos os CSVs padrao: aspas escapadas (`""`), quebras de linha internas de campos entre aspas, linhas maiores que `MAX_LINE` e falhas de alocacao.
- `strdup` depende de ambiente POSIX (gcc em Linux costuma aceitar). Pode precisar de ajuste no compilador MSVC do Windows.
- A comparacao por titulo normalizado pode misturar obras homonimas; para vinculacao confiavel, deve-se usar `imdb_id` quando presente.
- O contador `tamanho` mede chaves distintas, nao a quantidade total de registros quando existe chave duplicada.
- Validadores ainda nao verificam todas as propriedades estruturais (ordenacao BST, ponteiros pai na rubro-negra e contagens).
- `clock()` mede tempo de CPU e os valores de busca com poucos itens sao pequenos demais para uma comparacao de desempenho robusta.

## Executar

Entre na pasta contendo o Makefile e execute:

```sh
make
./testes
./testes_integracao
./sistema_integrado --buscar "The Last Command"
```

Em Windows, use MSYS2/MinGW com `mingw32-make` (ou compile manualmente com GCC). Os arquivos CSV devem permanecer na pasta `dados/`.
