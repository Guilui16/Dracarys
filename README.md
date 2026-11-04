# Dracarys
Trabalho de Estrutura de Dados - Árvore de Filmes indicados ao Oscar, Globo de Ouro, BAFTA e Critics' Choice Awards
# \[GOT Realness\]

> Entrega 1 de trabalho da disciplina Estruturas de Dados II — UNICID Prof. Cid Rodrigues de Andrade

## 👥 Integrantes do Grupo

| Nome completo | RA | GitHub |
|Guilherme Luiz Palhares|42808197|---|
|Gustavo de Siqueira Rodrigues|43446965|  |
|Gustavo do Rosario Trujillo|41944925|  |
|Rafael Miranda de Souza Lima|41765401|  |
|  |  |  |
|  |  |  |


## 1. Dataset


### 1.1 Descrição

O projeto utiliza dois conjuntos complementares de dados cinematográficos em CSV. O primeiro reúne registros históricos de indicações e vitórias do Oscar e do Globo de Ouro, contendo categorias, anos, indicados e filmes. O segundo reúne filmes cadastrados no IMDb, com identificador, título, ano e gêneros. A árvore AVL indexa as indicações, enquanto a árvore rubro-negra indexa o catálogo de filmes. O coletor foi programado para obter, quando houver acesso às fontes, pelo menos 50.001 linhas somando os dois conjuntos, sem duplicar artificialmente os registros. Essa soma não representa 50 mil indicações: inclui também filmes sem premiação. Até o momento, a coleta integral dos CSVs históricos ainda não foi validada neste ambiente. Para conferir a implementação, foram usados testes com dados sintéticos, identificados separadamente.


### 1.2 Fonte

Oscar: https://raw.githubusercontent.com/Sidodj/SQL-Oscar-project/main/data/the_oscar_award.csv  
Globo de Ouro: https://raw.githubusercontent.com/prasertcbs/basic-dataset/master/golden_globe_awards.csv  
IMDb (arquivos públicos não comerciais): https://developer.imdb.com/non-commercial-datasets/  
Dados de teste: gerados pelos próprios scripts testes.py e testes_integracao.py.  
BAFTA e Critics’ Choice são fontes previstas para ampliação, mas não integram o coletor atual.


### 1.3 Estrutura dos dados

Na AVL, cada registro de indicação inclui id (int), premiacao (string), ano_cerimonia (int), ano_filme (int ou vazio), categoria (string), indicado (string), filme (string), vencedor (0, 1 ou vazio), genero (string, quando vinculado), subgenero (vazio quando não documentado), imdb_id (string, quando identificado) e fonte (URL). A chave é o título normalizado do filme; na falta dele, o nome do indicado. Na rubro-negra, cada filme contém imdb_id (string), filme (string), ano_filme (int), genero (string), generos_imdb (string), subgenero (string ou vazio) e fonte (URL); a chave é o título normalizado. A normalização reduz diferenças de caixa e acentuação. Chaves repetidas armazenam listas de registros no mesmo nó, permitindo múltiplas indicações do mesmo filme ou títulos homônimos.

Campos de destaque:

• Chave de busca (string): título normalizado, usado nas duas árvores.

• Identificador (int/string): id de indicação ou imdb_id do filme.

• Metadados (strings/inteiros): premiação, categoria, ano, gênero e resultado.


### 1.4 Justificativa da escolha

O domínio apresenta registros compostos, nomes repetidos, categorias distintas, muitos anos e chaves textuais. Esse cenário permite avaliar colisões de chave, consultas e manutenção do balanceamento em volume elevado. A separação por tipo de registro preserva responsabilidades: a AVL facilita recuperar indicações por título e a rubro-negra disponibiliza metadados do catálogo. O total planejado supera 50 mil linhas combinadas, atendendo à proposta de escala do projeto, sujeito à execução e conferência da coleta real.


## 2. Estruturas de Árvore Escolhidas


### 2.1 Estruturas

Árvore AVL: árvore binária de busca autobalanceada. Em cada nó, a diferença entre as alturas da subárvore esquerda e da direita deve permanecer entre −1 e +1. Após inserções, rotações simples ou duplas restauram essa condição.  
Árvore rubro-negra: árvore binária de busca autobalanceada que marca os nós como vermelhos ou pretos. A raiz é preta, um nó vermelho não pode ter filho vermelho, e todos os caminhos de um nó às folhas sentinelas têm a mesma quantidade de nós pretos. Recolorações e rotações preservam essas propriedades.  
No sistema, as árvores não são classificadores preditivos: são índices de busca complementares.


### 2.2 Justificativa técnica

Ambas garantem busca e inserção com custo assintótico O(log n), mesmo no pior caso, ao contrário de uma BST não balanceada. A AVL oferece controle mais rigoroso da altura; a rubro-negra permite balanceamento menos rígido, frequentemente útil em cenários de atualização. A combinação foi escolhida para separar duas fontes de naturezas diferentes sem repetir os mesmos registros, permitindo pesquisar o histórico de premiações e os dados de um filme na mesma operação integrada. A associação segura entre linhas das duas fontes exige imdb_id verificado; títulos iguais, isoladamente, não confirmam identidade.


### 2.3 Operações implementadas (Entrega 2)

Inserção: IMPLEMENTADA nas duas árvores; registros com chave repetida são anexados à lista do nó.

Remoção: NÃO IMPLEMENTADA nos códigos atuais. Requer tratamento adicional de rebalanceamento.

Busca: IMPLEMENTADA nas duas árvores, por chave textual normalizada; retorna lista de registros.

Percursos pré-ordem, em ordem e pós-ordem: NÃO IMPLEMENTADOS como funções públicas nesta versão.

Balanceamento: IMPLEMENTADO; AVL usa alturas e rotações; rubro-negra usa rotações e recolorações.

Outras operações: altura(), validar(), contagem de chaves, leitura de CSV e consulta integrada por filme.


### 2.4 Complexidade

Complexidades teóricas das operações sobre chaves distintas. n representa o número de nós da árvore; k representa o número de registros armazenados sob a chave encontrada. No caso de títulos repetidos, a busca da chave custa O(log n), e devolver/copiar k registros pode acrescentar O(k). A remoção consta apenas como referência teórica, pois não foi implementada.


| Operação | Melhor caso | Caso médio | Pior caso |
|---|---|---|---|
| Inserção | O(1)* | O(log n) | O(log n) |
| Busca | O(1) | O(log n) | O(log n) |
| Remoção (teórica; não implementada) | O(1)* | O(log n) | O(log n) |

*O(1) descreve situações favoráveis, como uma árvore vazia ou encontrar a chave na raiz.

## 3. Plano de Testes


### 3.1 Objetivo dos testes

Verificar se as estruturas mantêm a ordem de busca e suas propriedades de balanceamento; se a inserção e a busca retornam os dados esperados; se valores repetidos não são perdidos; e se a AVL (indicações) e a rubro-negra (filmes) podem ser consultadas juntas. Também se pretende medir o tempo de construção e de busca com CSVs reais, após a coleta.


### 3.2 Cenários de teste


| # | Cenário | Entrada | Resultado esperado | Status |
|---|---|---|---|---|
| 1 | Inserção e busca | 50.000 chaves sintéticas por árvore | Invariantes e buscas corretas | Aprovado |
| 2 | Duplicidade / ausência | Registro repetido e chave inexistente | Lista com 2 registros e lista vazia | Aprovado |
| 3 | Integração | 16.001 + 35.000 simulados | Consulta complementar e 51.001 registros | Aprovado |


### 3.3 Casos extremos (edge cases)

Foram contemplados: busca por chave inexistente (retorna lista vazia), inserção de registro duplicado (preserva os dois registros sob uma única chave), inspeção das invariantes de balanceamento e consulta de uma chave presente nas duas estruturas. A execução atual também insere 50.000 chaves únicas em ordem pseudoaleatória reprodutível (semente 2026). Ainda devem ser acrescentados testes específicos de entrada vazia, um único elemento, ordenação estritamente crescente/decrescente e remoção, quando esta for implementada.


### 3.4 Testes de desempenho (Entrega 2)

Os testes disponíveis executam 50.000 inserções sintéticas em cada implementação e uma carga complementar de 16.001 registros de indicações simuladas e 35.000 filmes simulados. Os scripts testes.py e testes_integracao.py validam estrutura e resultados; não registram tempos nem consumo de memória. O módulo sistema_integrado.py utiliza time.perf_counter() para medir o tempo de montagem de cada árvore e de uma consulta, mas sua execução depende dos CSVs reais gerados por preparar_dados.py. Portanto, ainda não há benchmark de tempos e memória da base histórica confirmado para relatar.


### 3.5 Resultados obtidos (Entrega 2)

Execuções locais confirmadas em 04/11/2026: testes.py — AVL com 50.000 chaves sintéticas, altura 19, validação aprovada; rubro-negra com 50.000 chaves sintéticas, altura 19, validação aprovada. testes_integracao.py — AVL com 16.001 indicações simuladas (altura 14) e rubro-negra com 35.000 filmes simulados (altura 28), totalizando 51.001 registros simulados; consulta complementar aprovada. Esses resultados atestam o funcionamento das implementações nesses testes, mas não comprovam a execução sobre 50.001 registros reais ou desempenho cronometrado. A análise de tempos e memória permanece pendente.


## 4. Como Executar


### 4.1 Pré-requisitos (Entrega 2)

Python 3.10 ou superior, acesso à internet para obter os arquivos originais e espaço para baixar o conjunto IMDb compactado. Os scripts utilizam apenas módulos da biblioteca padrão do Python (csv, gzip, urllib, pathlib, argparse e time, entre outros), dispensando bibliotecas externas. Usar terminal integrado do VS Code ou terminal do sistema.


### 4.2 Instruções (Entrega 2)

No terminal, execute os comandos abaixo na ordem apresentada:

```bash
git clone https://github.com/Guilui16/Dracarys.git
cd Dracarys/cinema_project
python preparar_dados.py
python testes.py
python testes_integracao.py
python sistema_integrado.py --buscar "The Last Command"
No Windows, se o comando python não funcionar, tente py. O coletor grava os CSVs e o resumo em dados/. A coleta completa não foi validada neste documento.
```


### 4.3 Estrutura do repositório (organização atual)

cinema_project/avl.py — classe da árvore AVL e operações de consulta.

cinema_project/rubro_negra.py — classe da árvore rubro-negra e operações de consulta.

cinema_project/sistema_integrado.py — montagem e busca nas duas árvores.

cinema_project/preparar_dados.py — coleta e preparação dos CSVs; comum.py — leitura e utilidades; testes.py e testes_integracao.py — verificações.

cinema_project/dados/ — amostra e arquivos CSV após coleta; cinema_project/LEIA_ME.md — documentação inicial. Pastas src/, dataset/, testes/ e resultados/ constam como sugestão do modelo do professor, mas não fazem parte da estrutura atual.


## 5. Referências

ACADEMY OF MOTION PICTURE ARTS AND SCIENCES. Awards Database. https://awardsdatabase.oscars.org/  
GOLDEN GLOBES. Awards Database. https://goldenglobes.com/awards-database/  
IMDb. Non-Commercial Datasets. https://developer.imdb.com/non-commercial-datasets/  
GITHUB. Sidodj/SQL-Oscar-project: the_oscar_award.csv. https://github.com/Sidodj/SQL-Oscar-project  
GITHUB. prasertcbs/basic-dataset: golden_globe_awards.csv. https://github.com/prasertcbs/basic-dataset  
CORMEN, T. H. et al. Introduction to Algorithms. 4. ed. MIT Press, 2022.  
REPOSITÓRIO DO GRUPO. https://github.com/Guilui16/Dracarys/tree/main/cinema_project
