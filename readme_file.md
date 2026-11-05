# Dracarys
Trabalho de Estrutura de Dados — Árvore de Filmes indicados ao Oscar, Globo de Ouro e Catálogo IMDb (Implementação em Linguagem C)
# [GOT Realness]

> Entrega 2 de trabalho da disciplina Estruturas de Dados II — UNICID Prof. Cid Rodrigues de Andrade

## 👥 Integrantes do Grupo

| Nome completo | RA | Github |
|---|---|---|
| Guilherme Luiz Palhares | 42808197 | Guiluiz16 |
| Gustavo de Siqueira Rodrigues | 43446965 | siqueira180 |
| Gustavo do Rosario Trujillo | 41944925 | — |

---

## 1. Dataset

### 1.1 Descrição
O projeto utiliza dois conjuntos complementares de dados cinematográficos em CSV. O primeiro reúne registros históricos de indicações e vitórias do Oscar e do Globo de Ouro (`indicacoes_reais.csv`), contendo categorias, anos, indicados e filmes. O segundo reúne filmes cadastrados no IMDb (`catalogo_filmes.csv`), com identificador, título, ano e gêneros. A árvore AVL indexa as indicações, enquanto a árvore rubro-negra indexa o catálogo de filmes. O sistema foi estruturado para processar mais de 50.000 linhas combinadas, separando responsabilidades sem duplicar artificialmente os registros.

### 1.2 Fonte
* Oscar: `https://raw.githubusercontent.com/Sidodj/SQL-Oscar-project/main/data/the_oscar_award.csv`
* Globo de Ouro: `https://raw.githubusercontent.com/prasertcbs/basic-dataset/master/golden_globe_awards.csv`
* IMDb (arquivos públicos não comerciais): `https://developer.imdb.com/non-commercial-datasets/`
* Dados de teste: gerados pelos próprios módulos executáveis e de integração em C (`testes.c` e `testes_integracao.c`).

### 1.3 Estrutura dos dados
Os registros são lidos e tratados em C através de estruturas (`Registro`) definidas nos módulos do sistema:
* **`id` / `imdb_id` (string)**: Identificadores únicos de indicação ou de catálogo IMDb.
* **`premiacao`, `ano_cerimonia`, `ano_filme`, `categoria`, `indicado`, `filme`, `vencedor`**: Metadados textuais e temporais das premiações.
* **`genero`, `subgenero`, `fonte`**: Classificações e referências de origem.

### 1.4 Justificativa da escolha
O domínio apresenta registros compostos, nomes repetidos, categorias distintas e chaves textuais, permitindo avaliar colisões de chave, consultas e manutenção do balanceamento em volume elevado. A implementação nativa em C garante controle estrito de alocação de memória dinâmica (`malloc`/`realloc`), ponteiros e desempenho de baixo nível adequado para grandes volumes de dados.

---

## 2. Estrutura(s) de Árvore Escolhida(s)

### 2.1 Estrutura(s)
* **Árvore AVL**: Árvore binária de busca autobalanceada por altura. Em cada nó, a diferença entre as alturas da subárvore esquerda e da direita (fator de balanceamento) permanece entre $-1$ e $+1$, mantida através de rotações simples e duplas.
* **Árvore Rubro-Negra**: Árvore binária de busca autobalanceada por cores (vermelho ou preto) e nós sentinelas (`NIL`), garantindo que nenhum nó vermelho tenha filhos vermelhos e que todos os caminhos tenham a mesma altura negra.

### 2.2 Justificativa técnica
Ambas garantem busca e inserção eficientes com custo assintótico $O(\log n)$ no caso médio e pior caso. A combinação separa duas fontes de naturezas diferentes (indicações com chaves repetidas vs. catálogo de filmes com chaves únicas), permitindo consultas integradas em C com alto desempenho e baixo consumo de recursos.

### 2.3 Operações implementadas (Entrega 2)
* **Inserção**: Implementada nas duas árvores com suporte a múltiplos registros por chave (anexados em vetores dinâmicos).
* **Remoção**: Não implementada na versão atual do código em C.
* **Busca**: Implementada por chave textual normalizada, retornando a lista de registros correspondentes.
* **Percursos**: Pré-ordem, em ordem e pós-ordem disponíveis de forma interna pelas rotinas de validação e percorrimento de nós.
* **Balanceamento**: AVL utiliza controle de altura e rotações; Rubro-Negra utiliza rotações e recolorações.
* **Outras operações**: `altura()`, `validar()`, leitura segura de CSVs com tratamento de aspas (`common.c`), contagem de chaves e busca integrada.

### 2.4 Complexidade
Complexidades teóricas das operações sobre chaves distintas ($n$ representa o número de nós; $k$ o número de registros armazenados sob a chave).

| Operação | Melhor caso | Caso médio | Pior caso |
|---|---|---|---|
| Inserção | $O(1)$* | $O(\log n)$ | $O(\log n)$ |
| Busca | $O(1)$ | $O(\log n)$ | $O(\log n)$ |
| Remoção (teórica) | $O(1)$* | $O(\log n)$ | $O(\log n)$ |

*\* $O(1)$ descreve situações favoráveis, como inserção em árvore vazia ou chave encontrada na raiz.*

---

## 3. Plano de Testes

### 3.1 Objetivo dos testes
Validar a corretude das implementações em C, a integridade estrutural das árvores após grandes volumes de inserções, a corretude das buscas com chaves duplicadas e a execução correta do sistema integrado.

### 3.2 Cenários de teste

| # | Cenário | Entrada | Resultado esperado | Status |
|---|---|---|---|---|
| 1 | Inserção e busca em massa | 50.000 chaves sintéticas por árvore (`testes.c`) | Invariantes de balanceamento e buscas corretas validadas com sucesso | Aprovado |
| 2 | Duplicidade e ausência | Registro repetido e chave inexistente | Retorno correto de múltiplos registros ou lista vazia | Aprovado |
| 3 | Integração de tipos | 16.001 indicações e 35.000 filmes simulados (`testes_integracao.c`) | Validação bem-sucedida do total combinado de 51.001 registros | Aprovado |

### 3.3 Casos extremos (edge cases)
Foram contemplados: busca por chave inexistente (retorna lista vazia), inserção de registro duplicado (preserva múltiplos registros sob a mesma chave por meio de realocação dinâmica), verificação das invariantes de balanceamento por altura/cor e testes de carga com 50.000 chaves pseudoaleatórias.

### 3.4 Testes de desempenho (Entrega 2)
Os testes implementados em C executam 50.000 inserções sintéticas em cada árvore e validam a estrutura por meio de asserções rígidas (`assert`). O programa principal (`sistema_integrado.c`) utiliza a biblioteca nativa `<time.h>` (`clock()`) para mensurar o tempo exato de montagem dos índices e de execução das consultas.

### 3.5 Resultados obtidos (Entrega 2)
Testes locais executados e aprovados:
* **`testes.c`**: Validação de 50.000 chaves na AVL e na Rubro-Negra com verificações de integridade estrutural aprovadas.
* **`testes_integracao.c`**: Simulação validada de 16.001 indicações e 35.000 filmes, totalizando 51.001 registros combinados sem falhas de ponteiros ou vazamentos de memória.

---

## 4. Como Executar

### 4.1 Pré-requisitos (Entrega 2)
* Compilador GCC compatível com C99 ou superior.
* Utilitário `make`.
* Sistema operacional Linux, macOS ou Windows (via WSL / MSYS2 / MinGW).

### 4.2 Instruções (Entrega 2)
No terminal, na raiz do repositório, execute os comandos abaixo:

```bash
git clone https://github.com/Guilui16/Dracarys.git
cd Dracarys/cinema_project

# Compilar todos os módulos usando o Makefile
make

# Executar os testes unitários e de estresse (50 mil chaves)
./testes

# Executar o teste de integração (51.001 registros)
./testes_integracao

# Executar o sistema integrado de busca por título
./sistema_integrado --buscar "The Last Command"
```

### 4.3 Estrutura do repositório (implementação em C)
* `common.h` / `common.c` — Funções utilitárias, normalização de strings e parser robusto de CSV.
* `avl.h` / `avl.c` — Implementação da Árvore AVL balanceada por altura.
* `rubro_negra.h` / `rubro_negra.c` — Implementação da Árvore Rubro-Negra com sentinela `NIL`.
* `sistema_integrado.c` — Leitura dos CSVs, montagem das árvores e consulta cruzada.
* `testes.c` — Testes de carga com 50.000 chaves.
* `testes_integracao.c` — Teste de integração de 51.001 registros.
* `Makefile` — Automação da compilação GCC.
* `dados/` — Pasta contendo os arquivos CSV de entrada (`indicacoes_reais.csv` e `catalogo_filmes.csv`).

---

## 5. Referências

ACADEMY OF MOTION PICTURE ARTS AND SCIENCES. Awards Database. `https://awardsdatabase.oscars.org/`  
GOLDEN GLOBES. Awards Database. `https://goldenglobes.com/awards-database/`  
IMDb. Non-Commercial Datasets. `https://developer.imdb.com/non-commercial-datasets/`  
GITHUB. Sidodj/SQL-Oscar-project: the_oscar_award.csv. `https://github.com/Sidodj/SQL-Oscar-project`  
GITHUB. prasertcbs/basic-dataset: golden_globe_awards.csv. `https://github.com/prasertcbs/basic-dataset`  
CORMEN, T. H. et al. Introduction to Algorithms. 4. ed. MIT Press, 2022.  
REPOSITÓRIO DO GRUPO. `https://github.com/Guilui16/Dracarys/tree/main/cinema_project`