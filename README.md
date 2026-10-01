# Dracarys
Trabalho de Estrutura de Dados - Árvore de Filmes indicados ao Oscar, Globo de Ouro, BAFTA e Critics' Choice Awards
# \[Dracarys\]

> Entrega 1 de trabalho da disciplina Estruturas de Dados II — UNICID Prof. Cid Rodrigues de Andrade

## 👥 Integrantes do Grupo

| Guilherme Luiz Palhares | 42808197 |

---

## 1\. Dataset

### 1.1 Descrição

O conjunto de dados reúne informações históricas detalhadas de grandes premiações da indústria cinematográfica (como Oscar, Globo de Ouro, BAFTA e Critics' Choice Awards). O domínio do problema é a análise preditiva e estatística de premiações de cinema. O formato do arquivo é CSV (Comma-Separated Values), contendo um volume estimado entre 50.000 e 100.000 registros, abrangendo múltiplas décadas de indicações, categorias, filmes, gêneros e subgêneros.

### 1.2 Fonte

Origem: Bases públicas agregadas e APIs especializadas em cinema (como TMDB - The Movie Database e datasets históricos do Kaggle sobre premiações de cinema, combinados e expandidos para atender à volumetria mínima).

### 1.3 Estrutura dos dados

Os campos e atributos relevantes estruturados nas colunas do dataset incluem:

id (int): Chave primária única para cada linha/registro.
ano (int): Ano da edição da premiação.
premiacao (string): Nome da premiação (ex: "Oscar", "Globo de Ouro", "BAFTA").
categoria (string): Categoria da indicação (ex: "Melhor Ator", "Melhor Filme", "Melhor Diretor").
indicado (string): Nome do filme, ator ou profissional indicado.
genero (string): Gênero principal do filme (ex: "Drama", "Ação", "Comédia").
subgenero (string): Subgênero ou temática do filme (ex: "Ficção Científica", "Biografia", "Suspense").
vencedor (int / boolean): Variável-alvo (target) indicando se o indicado venceu a categoria (1 para Sim, 0 para Não).

### 1.4 Justificativa da escolha

Este dataset é ideal porque combina dados categóricos de alta cardinalidade com um volume robusto capaz de testar a eficiência computacional, o particionamento de nós e a profundidade de árvores de decisão. A diversidade de atributos (categóricos e numéricos) permite avaliar o ganho de informação (Information Gain) e a pureza dos nós de forma realista.

---

## 2\. Estrutura(s) de Árvore Escolhida(s)

### 2.1 Estrutura(s)

Árvore de Decisão (Decision Tree / CART - Classification and Regression Trees) implementada a partir de conceitos fundamentais de nós, partições e recursividade, podendo ser complementada com uma Árvore Binária de Busca (BST) para indexação e recuperação rápida dos metadados dos filmes.

### 2.2 Justificativa técnica

A Árvore de Decisão foi escolhida por refletir diretamente a lógica de tomada de decisão do problema (classificar se uma obra/pessoa será vencedora com base em critérios como categoria e gênero). Para a parte puramente estrutural de manipulação em memória exigida pela disciplina, o uso de nós encadeados em estruturas de árvore permite compreender a complexidade de percursos, recursão e critérios de parada (como pureza de Gini ou Entropia).

### 2.3 Operações implementadas (Para Entrega 2\)

- [ ] Inserção  
- [ ] Remoção  
- [ ] Busca  
- [ ] Percursos (pré-ordem, em ordem, pós-ordem)  
- [ ] Balanceamento (se aplicável)  
- [ ] Outra: \_\_\_\_\_\_

### 2.4 Complexidade

Tabela com a complexidade assintótica (Big-O) teórica de cada operação implementada, no melhor, médio e pior caso.

| Operação | Melhor caso | Caso médio | Pior caso |
| :---- | :---- | :---- | :---- |
| Inserção | $O(1)$ | $O(\log n)$ | $O(n)$ |
| Busca | $O(1)$ | $O(\log n)$ | $O(n)$ |
| Remoção | $O(1)$ | $O(\log n)$ | $O(n)$ |

---

## 3\. Plano de Testes

### 3.1 Objetivo dos testes

Validar a corretude na divisão dos nós, a acurácia do modelo preditivo, o desempenho de tempo de execução e o consumo de memória ao processar grandes volumes de dados (até 1 milhão de registros), além de verificar o comportamento em cenários de dados extremos.

### 3.2 Cenários de teste

| \# | Cenário | Entrada | Resultado esperado | Status |
| :---- | :---- | :---- | :---- | :---- |
| 1 | Conjunto padrão de dados | CSV com 50.000 linhas | Árvore construída sem estouro de pilha | ☐ |
| 2 | Consulta de elemento existente | ID ou atributos de um indicado real | Retorno correto da predição (Vencedor/Não) | ☐ |
| 3 | Consulta de elemento inexistente | Chave fora do escopo | Tratamento adequado de erro / Null | ☐ |

### 3.3 Casos extremos (edge cases)

Árvore vazia (sem registros carregados).
Conjunto de dados contendo apenas um único elemento.
Dados duplicados ou com atributos totalmente idênticos, mas resultados diferentes.
Dados inseridos em ordem estritamente crescente ou decrescente (teste de degradação da estrutura auxiliar).
Volume máximo do dataset (escala de 500k a 1 milhão de registros para estresse de memória).

### 3.4 Testes de desempenho (Para Entrega 2\)

Descreva como o grupo mediu tempo de execução e/ou uso de memória, e com quais tamanhos de entrada (ex: 100, 1.000, 10.000 registros).

### 3.5 Resultados obtidos (Para Entrega 2\)

Resuma os resultados (tabelas, gráficos ou links para arquivos de saída na pasta `/resultados`) e compare-os com a complexidade assintótica (Big-O) teórica.

---

## 4\. Como Executar

### 4.1 Pré-requisitos (Para Entrega 2\)

Linguagem, versão e dependências necessárias.

### 4.2 Instruções (Para Entrega 2\)

\# Exemplo  
git clone \<link-do-repositorio\>  
cd \<pasta\>  
\# comandos de compilação/execução

### 4.3 Estrutura do repositório (já com pastas para a Entrega 2\)

/src         → código-fonte  
/dataset     → dataset utilizado  
/testes      → scripts e casos de teste  
/resultados  → saídas e relatórios de desempenho  
README.md  
---

## 5\. Referências

CORMEN, Thomas H. et al. Algoritmos: Teoria e Prática. 3ª Edição. Campus, 2012.
WATERS, Raúl. Data Structures and Algorithms Analysis. Academic Press, 2020.
Documentação oficial do Scikit-learn (Módulo de Decision Trees). Disponível em: https://scikit-learn.org/.
