# Dracarys — GOT Realness

**Estruturas de Dados II | Entrega 1 e Entrega 2 | UNICID**  
**Professor:** Cid Rodrigues de Andrade  
**Tema:** organização e consulta de dados cinematográficos com árvores AVL e rubro-negra em **C**.

> **Escopo real do trabalho:** as árvores são **árvores binárias de busca autobalanceadas**, não classificadores de aprendizado de máquina. O sistema não prevê o vencedor de uma premiação: ele indexa e pesquisa registros.

## Integrantes do grupo

| Integrante | RA | GitHub |
|---|---|---|
| Guilherme Luiz Palhares | 42808197 | `Guiluiz16` |
| Gustavo de Siqueira Rodrigues | 43446965 | `siqueira180` |
| Gustavo do Rosario Trujillo | 41944925 | Não informado |

**Repositório:** https://github.com/Guilui16/Dracarys/tree/main/cinema_project  
**Atenção:** o link do repositório contém `Guilui16`, enquanto o identificador informado na tabela anterior é `Guiluiz16`. Conferir o nome de usuário antes da entrega.

---

## Resumo do projeto

O projeto usa duas estruturas complementares:

- **AVL:** indexa registros de **indicações e vencedores**, permitindo buscar várias indicações associadas a um título.
- **Rubro-negra:** indexa o **catálogo de filmes**, com título, ano, gênero, identificador IMDb e outros metadados.
- **Sistema integrado:** normaliza um título pesquisado, consulta ambas as estruturas e mostra as informações encontradas em cada conjunto.

A implementação foi refeita em **linguagem C**, com arquivos `.c` e `.h`, compilação via **GCC** e um `Makefile`. A versão revisada foi compilada e executada em **05/11/2026**.

**Situação dos dados:** os arquivos CSV disponibilizados para a revisão contêm **5 registros de indicações** e **3 registros de catálogo**, ou **8 linhas de dados ao todo**. O funcionamento com grandes volumes foi validado com **registros sintéticos gerados em memória**. **Ainda não foi validada uma base histórica real de 50.000 registros ou mais.** Não confundir volume de testes com volume histórico coletado.

---

# 1. Dataset

## 1.1 Descrição

O domínio do problema é o **cinema**, especialmente a organização de registros de premiações e de um catálogo de filmes. Há dois arquivos CSV, com estruturas diferentes e complementares:

1. `dados/indicacoes_reais.csv`: registros de indicação e vitória; contém campos como premiação, cerimônia, categoria, pessoa indicada, filme, vencedor e fonte.
2. `dados/catalogo_filmes.csv`: registros de filmes, com identificador IMDb, título, ano, gênero principal, gêneros IMDb, possível subgênero e fonte.

Esses arquivos são **amostras** da base proposta. O conjunto planejado poderá reunir registros de Oscar e Globo de Ouro, complementados por um catálogo IMDb. **BAFTA e Critics' Choice Awards são possibilidades de expansão**, mas não há evidência de que seus registros estejam incorporados aos dois CSVs revisados.

As árvores mantêm **índices diferentes**: um mesmo título pode aparecer em ambos, mas a ocorrência na AVL representa uma indicação e a ocorrência na rubro-negra representa um filme. A soma de linhas dos dois arquivos não é igual ao número de filmes únicos e tampouco ao número de indicações.

## 1.2 Fonte e procedência

| Conjunto | Fonte indicada nos arquivos/projeto | Observação |
|---|---|---|
| Oscar | https://github.com/Sidodj/SQL-Oscar-project | Fonte informada nos registros de amostra |
| Globo de Ouro | https://github.com/prasertcbs/basic-dataset | Fonte pesquisada na etapa de levantamento; não consta nos oito registros da amostra revisada |
| Catálogo IMDb | https://developer.imdb.com/non-commercial-datasets/ | Dados IMDb disponíveis sob condições de uso não comercial |
| Base oficial do Oscar | https://awardsdatabase.oscars.org/ | Referência para conferência histórica |
| Base oficial do Globo de Ouro | https://goldenglobes.com/awards-database/ | Referência para conferência histórica |
| Testes de volume | `testes.c` e `testes_integracao.c` | Registros **simulados** e gerados dentro dos próprios programas |

O campo `fonte` de cada registro CSV permite registrar sua origem. A revisão executada verificou o **comportamento do código com os CSVs recebidos**; não foi feita auditoria histórica independente de cada linha.

## 1.3 Estrutura dos dados

Os campos do tipo `Registro` são declarados em `common.h`. **Na implementação C revisada, eles são armazenados como arrays de caracteres** (`char[256]`), inclusive campos que representam anos ou respostas binárias; não são `int` no `struct`.

| Campo | Tipo em C | Uso |
|---|---|---|
| `id` | `char[256]` | Identificador da indicação |
| `premiacao` | `char[256]` | Ex.: Oscar |
| `ano_cerimonia` | `char[256]` | Ano da cerimônia |
| `ano_filme` | `char[256]` | Ano do filme |
| `categoria` | `char[256]` | Categoria da premiação |
| `indicado` | `char[256]` | Pessoa ou obra indicada |
| `filme` | `char[256]` | Título do filme |
| `vencedor` | `char[256]` | Valor textual `1`, `0` ou vazio |
| `genero` | `char[256]` | Gênero cinematográfico |
| `generos_imdb` | `char[256]` | Lista/classificação IMDb, quando presente |
| `subgenero` | `char[256]` | Complemento de gênero, quando documentado |
| `imdb_id` | `char[256]` | Identificador IMDb, quando disponível |
| `fonte` | `char[256]` | Endereço da fonte |
| `status_genero` | `char[256]` | Proveniência/estado do preenchimento de gênero |
| `tipo` | `char[256]` | `indicacao` ou `filme`, preenchido durante a leitura |

### Chave de busca

As duas árvores usam uma **string derivada do título**, processada por `normalizar_string()` em `common.c`. A rotina converte caracteres alfanuméricos para minúsculas e separa blocos por espaços. **Ela não é uma transliteração Unicode completa**: acentos UTF-8 e outros caracteres especiais podem produzir normalizações imperfeitas.

- **AVL:** utiliza o título do filme; se estiver vazio, utiliza o nome do indicado. Se ambos estiverem vazios, usa uma chave alternativa baseada no ID.
- **Rubro-negra:** utiliza o título do filme.
- **Duplicidade:** se a chave já existe, o programa acrescenta o registro à **lista dinâmica do mesmo nó**, em vez de criar outro nó com a mesma chave.
- **Limitação:** títulos homônimos de anos diferentes podem compartilhar o mesmo nó; título igual nas duas fontes **não é prova suficiente** de que se trata da mesma obra. Para associação robusta, deve-se comparar o identificador IMDb, quando disponível.

### Exemplos das amostras

Os arquivos recebidos incluem o título **The Last Command**: duas indicações na AVL e um registro de filme na rubro-negra. Também incluem **Gone with the Wind**: uma indicação e um filme no catálogo. Esses exemplos permitem demonstrar a busca conjunta.

## 1.4 Justificativa da escolha

O domínio cinematográfico fornece **registros compostos**, atributos heterogêneos, muitos anos e categorias, títulos repetidos e informações distribuídas em fontes diferentes. Isso permite avaliar:

- inserção de chaves textuais;
- organização de múltiplos registros sob uma chave;
- busca e recuperação de resultados;
- balanceamento após milhares de inserções;
- integração de índices com diferentes responsabilidades.

A escolha de duas árvores distintas é didática: permite demonstrar **duas estratégias de autobalanceamento** dentro de uma aplicação integrada. A meta acadêmica de **50 mil a 1 milhão de registros reais** continua condicionada à obtenção e validação dos arquivos completos.

---

# 2. Estruturas de árvore escolhidas

## 2.1 Estruturas e funcionamento

### Árvore AVL — `avl.c` / `avl.h`

É uma árvore binária de busca autobalanceada. Para cada nó, o **fator de balanceamento** é a altura da subárvore esquerda menos a altura da direita e deve permanecer entre **−1 e +1**. Ao inserir uma chave, o código atualiza alturas e aplica rotações **à esquerda** ou **à direita**, incluindo casos de rotação dupla, quando necessário.

No projeto, cada nó AVL contém a **chave**, os ponteiros `esquerda` e `direita`, a `altura` e um vetor dinâmico de registros de indicação associados à chave. A função `inserir_avl()` acrescenta registros e `buscar_avl()` retorna os registros da chave encontrada.

### Árvore rubro-negra — `rubro_negra.c` / `rubro_negra.h`

É uma árvore binária de busca autobalanceada cujos nós são **vermelhos** ou **pretos**. As propriedades principais são: a raiz deve ser preta; as folhas sentinelas `NIL` são pretas; nó vermelho não tem filho vermelho; e todos os caminhos de um nó até suas folhas `NIL` possuem a mesma quantidade de nós pretos.

A inserção cria um nó vermelho e, se necessário, a função de correção executa **recolorações e rotações**. A árvore usa ponteiro para `pai` e um nó sentinela compartilhado `NIL`. Também armazena uma lista dinâmica para chaves repetidas. No projeto, ela indexa **filmes e gêneros**.

### Diferença entre as árvores

| Característica | AVL | Rubro-negra |
|---|---|---|
| Critério de equilíbrio | Diferença das alturas | Propriedades de cores e altura negra |
| Correções | Rotações simples/duplas | Recolorações e rotações |
| Estrutura de nó | Altura, filhos e registros | Cor, pai, filhos e registros |
| Uso neste projeto | Indicações/vitórias | Catálogo de filmes |
| Busca por chave, pior caso | `O(log n)` | `O(log n)` |

## 2.2 Justificativa técnica

Ambas evitam a degradação da altura típica de uma árvore binária de busca simples em certos padrões de inserção. A AVL impõe balanceamento mais rigoroso; a rubro-negra utiliza restrições de cores e pode ter altura maior, ainda dentro de `O(log n)`.

**Importante:** como as estruturas armazenam **conjuntos diferentes** neste projeto, seus tempos não representam uma comparação controlada de desempenho entre AVL e rubro-negra para a mesma base. Os testes individuais com chaves simuladas iguais são os mais indicados para comparações diretas de algoritmos.

## 2.3 Operações implementadas (Entrega 2)

| Operação | AVL | Rubro-negra | Situação |
|---|---|---|---|
| Criar árvore | `criar_avl()` | `criar_rn()` | Implementada |
| Inserir | `inserir_avl()` | `inserir_rn()` | Implementada |
| Buscar | `buscar_avl()` | `buscar_rn()` | Implementada |
| Agrupar chave duplicada | Sim | Sim | Implementada |
| Balancear após inserção | Altura + rotações | Recoloração + rotações | Implementada |
| Obter altura | `altura_avl()` | `altura_rn()` | Implementada |
| Validar propriedades básicas | `validar_avl()` | `validar_rn()` | Implementada, mas não exaustiva |
| Liberar memória dos nós | `liberar_avl()` | `liberar_rn()` | Implementada |
| Remover uma chave | — | — | **Não implementada** |
| Percursos públicos em pré-ordem, ordem e pós-ordem | — | — | **Não implementados** |

O código possui funções recursivas internas para validação/altura/liberação, porém isso **não equivale** à implementação de funções públicas de percurso para listar as chaves.

## 2.4 Complexidade assintótica (Big-O)

Seja `n` o número de **chaves distintas (nós)** na árvore e `k` a quantidade de registros guardados sob uma mesma chave.

| Operação | Melhor caso | Caso médio | Pior caso |
|---|---|---|---|
| Busca por chave | `O(1)` | `O(log n)` | `O(log n)` |
| Inserção de chave | `O(1)` em árvore vazia | `O(log n)` | `O(log n)` |
| Inclusão de registro em chave já existente | `O(1)` se a chave está na raiz e há capacidade | `O(log n)` | `O(log n + k)` em realocação/cópia do vetor |
| Altura (`altura_avl`) | `O(1)` | `O(1)` | `O(1)` |
| Altura (`altura_rn`) | `O(n)` | `O(n)` | `O(n)` |
| Validação estrutural | `O(n)` | `O(n)` | `O(n)` |
| Liberação de todos os nós | `O(n + r)` | `O(n + r)` | `O(n + r)` |
| Remoção | — | — | **Não implementada** (em teoria, `O(log n)`) |

`r` é a quantidade total de registros nas listas dos nós. Ao retornar o ponteiro para o vetor de `k` registros, a busca não copia automaticamente esses registros; iterar e imprimir os `k` resultados adiciona custo `O(k)`. As complexidades por chave assumem comparação de strings de comprimento limitado por `MAX_STR`.

---

# 3. Plano de testes

## 3.1 Objetivos

Verificar inserções, buscas, tratamento de chaves duplicadas, contagem de chaves distintas, manutenção das propriedades de balanceamento e integração dos dois conjuntos. Os testes com valores sintéticos verificam **corretude estrutural e escala**; os CSVs de amostra permitem conferir a **leitura e a consulta integrada**.

## 3.2 Ambiente de execução verificado

- **Data da reexecução:** 05/11/2026.
- **Linguagem:** C.
- **Compilador:** GCC 14.2.0 em ambiente Linux.
- **Opções de compilação:** `-Wall -Wextra -O2`.
- **Comandos:** `make clean`, `make`, `./testes`, `./testes_integracao` e duas consultas com `./sistema_integrado`.
- **Arquivos de entrada reais da revisão:** 5 indicações e 3 filmes, distribuídos em dois CSVs pequenos; **não representam os 50 mil registros reais pretendidos**.

## 3.3 Cenários e resultados executados

| Nº | Programa/cenário | Entrada | Resultado obtido | Status |
|---:|---|---|---|---|
| 1 | Compilação | Três executáveis com GCC | Compilação concluída após correção de `CLOCKS_PER_SEC` | **Aprovado na versão revisada** |
| 2 | AVL: inserir e buscar | 50.000 chaves sintéticas sequenciais | 50.000 chaves; altura **16**; validação aprovada | **Aprovado** |
| 3 | Rubro-negra: inserir e buscar | 50.000 chaves sintéticas sequenciais | 50.000 chaves; altura **29**; validação aprovada | **Aprovado** |
| 4 | Duplicidade na AVL | Inserção adicional da chave `000017` | 2 registros sob a mesma chave; contagem de nós permanece 50.000 | **Aprovado** |
| 5 | Duplicidade na rubro-negra | Inserção adicional da chave `000017` | 2 registros sob a mesma chave; contagem de nós permanece 50.000 | **Aprovado** |
| 6 | Teste de integração | 16.001 indicações + 35.000 filmes **simulados** | **51.001 registros sintéticos**, consulta complementar validada | **Aprovado** |
| 7 | AVL no teste integrado | 16.001 chaves sintéticas | Altura **14** | **Aprovado** |
| 8 | Rubro-negra no teste integrado | 35.000 chaves sintéticas | Altura **28** | **Aprovado** |
| 9 | Consulta integrada: `The Last Command` | CSVs recebidos | **2 indicações** e **1 filme** | **Aprovado** |
| 10 | Consulta integrada: `Gone with the Wind` | CSVs recebidos | **1 indicação** e **1 filme** | **Aprovado** |
| 11 | Sanitizadores (revisão prévia) | Consulta dos CSVs de amostra | AddressSanitizer/UndefinedBehaviorSanitizer sem alertas na execução relatada | **Aprovado nessa execução limitada** |
| 12 | Carregamento de 50.000+ linhas históricas reais | Base completa | Não foi realizado com uma base histórica validada | **Pendente** |

**Nota:** o arquivo `testes.c` usa `assert()`; para que as verificações sejam executadas, **não compile com `-DNDEBUG`**. `testes_integracao.c` também usa `assert()`.

## 3.4 Saída obtida — testes individuais

```text
ArvoreAVL OK, 50000 chaves, altura: 16
ArvoreRubroNegra OK, 50000 chaves, altura: 29
```

O teste cria chaves como `000000`, `000001` e assim sucessivamente. Também verifica a busca pela chave `000017` e adiciona outro registro à mesma chave, confirmando que os dois valores ficam armazenados no nó sem aumentar a quantidade de **chaves distintas**.

## 3.5 Saída obtida — integração com dados sintéticos

```text
OK: 51.001 registros simulados com tipos separados; consulta complementar validada.
AVL: 16.001 indicacoes simuladas | altura: 14
Rubro-negra: 35.000 filmes simulados | altura: 28
```

Neste caso, as estruturas são alimentadas com conjuntos **simulados**, distinguindo o campo `tipo` como `indicacao` ou `filme`. A busca de uma mesma chave nos dois índices verifica que os resultados recuperados pertencem aos tipos esperados.

## 3.6 Saída obtida — sistema com os CSVs anexados

Consulta a `The Last Command`:

```text
=== INDICES COMPLEMENTARES ===
AVL: 5 indicacoes | 3 chaves | altura 2 | montagem 0.0000s
Rubro-negra: 3 filmes | 3 chaves | altura 2 | montagem 0.0000s
TOTAL COMBINADO: 8 linhas (tipos diferentes)

Busca por titulo: 'The Last Command' (0.00000000s)
Indicacoes: 2 | Itens de catalogo: 1
  INDICACAO: Oscar 1929 Actor vencedor=1
  INDICACAO: Oscar 1929 Writing vencedor=0
  FILME: The Last Command 1927 Drama tt0019071
```

Consulta a `Gone with the Wind`:

```text
Busca por titulo: 'Gone with the Wind' (0.00000000s)
Indicacoes: 1 | Itens de catalogo: 1
  INDICACAO: Oscar 1940 Best Picture vencedor=1
  FILME: Gone with the Wind 1939 Drama tt0031381
```

Os tempos de montagem e busca aparecem arredondados para zero nessa amostra pequena; isso **não permite comparar desempenho** ou concluir que a operação tenha duração literalmente nula.

## 3.7 Casos extremos (edge cases)

| Caso | Situação atual |
|---|---|
| Inserir chaves em ordem crescente | Exercitado com 50.000 chaves nos testes individuais |
| Inserir chave repetida | Exercitado nas duas árvores |
| Consultar chave presente nas duas árvores | Exercitado no teste integrado |
| Árvore vazia e nó único | Funções preveem esses estados, mas faltam casos dedicados no conjunto de testes enviado |
| Buscar chave inexistente | Código retorna ponteiro `NULL` e contagem zero; faltou um `assert` explícito nos testes enviados |
| Inserções estritamente decrescentes | Não há caso dedicado no arquivo de testes enviado |
| Títulos iguais para filmes diferentes | Agrupados na mesma chave; desambiguação por IMDb é melhoria futura |
| Remoção de nó/chave | Não implementada |
| CSV com aspas escapadas, campos multilinha ou linhas muito longas | Parser atual não cobre plenamente essas situações |
| Falha de alocação de memória | Ainda falta tratamento completo para `malloc`/`realloc` |

## 3.8 Medição de desempenho (Entrega 2)

` sistema_integrado.c ` utiliza `clock()` da biblioteca `<time.h>` e converte os valores por `CLOCKS_PER_SEC` para exibir o **tempo de CPU** de montagem da AVL, montagem da rubro-negra e pesquisa conjunta. Os programas `testes.c` e `testes_integracao.c` não produzem métricas cronometradas; eles medem quantidade de chaves, altura e verificações de corretude.

**Limite:** com apenas 8 linhas reais da amostra, os tempos exibidos são muito pequenos e não sustentam uma avaliação comparativa de desempenho. Para um benchmark acadêmico mais robusto, recomenda-se executar com CSVs completos e repetir buscas/inserções por lotes, registrando média e variação do tempo, tamanho da entrada e uso de memória.

## 3.9 Interpretação dos resultados

Os testes mostram que as duas árvores mantiveram as propriedades básicas verificadas em volumes sintéticos elevados. A AVL ficou com altura **16** e a rubro-negra com altura **29** ao inserir 50.000 chaves em ordem crescente. Esse resultado **não comprova superioridade geral** de uma sobre a outra: as políticas de balanceamento são distintas, e a comparação de desempenho exige medir o mesmo conjunto e ambiente, com várias execuções.

Os 51.001 registros simulados confirmam a lógica de **complementaridade**. Já as consultas com CSVs demonstram que é possível reunir indicação e metadados de catálogo por título. **A associação entre títulos ainda não equivale a uma junção auditada por `imdb_id`.**

---

# 4. Como executar

## 4.1 Pré-requisitos

- Compilador **GCC** com suporte a C, preferencialmente em Linux, macOS ou Windows com **MSYS2/MinGW**.
- `make` (opcional, mas recomendado, pois há `Makefile`).
- Terminal do VS Code ou terminal do sistema.
- Dois arquivos CSV dentro da pasta `dados/`, com os nomes `indicacoes_reais.csv` e `catalogo_filmes.csv`.

**Não é necessário Python para compilar ou executar os programas C.** Python poderá ser usado separadamente para preparar uma base maior, caso se mantenha algum coletor da versão anterior, mas ele não faz parte da compilação C revisada.

## 4.2 Organização de arquivos esperada

```text
cinema_project/
├── README.md
├── Makefile
├── common.h
├── common.c
├── avl.h
├── avl.c
├── rubro_negra.h
├── rubro_negra.c
├── sistema_integrado.c
├── testes.c
├── testes_integracao.c
└── dados/
    ├── indicacoes_reais.csv
    └── catalogo_filmes.csv
```

**Observação:** esta é a estrutura de execução da **versão C revisada**. Confirme se os arquivos já foram substituídos no GitHub antes de afirmar que o repositório remoto tem exatamente esta organização.

## 4.3 Baixar o repositório

```bash
git clone https://github.com/Guilui16/Dracarys.git
cd Dracarys/cinema_project
```

Se os arquivos `.c`, `.h` e o `Makefile` estiverem em outra subpasta, acesse essa pasta antes de compilar. Execute os comandos a partir da pasta onde está o `Makefile` e onde existe a subpasta `dados/`.

## 4.4 Compilar com Make (Linux/macOS/MSYS2)

```bash
make clean
make
```

O `Makefile` cria três executáveis: `testes`, `testes_integracao` e `sistema_integrado`.

### Alternativa: compilação direta com GCC

```bash
gcc -Wall -Wextra -O2 -o testes testes.c common.c avl.c rubro_negra.c
gcc -Wall -Wextra -O2 -o testes_integracao testes_integracao.c common.c avl.c rubro_negra.c
gcc -Wall -Wextra -O2 -o sistema_integrado sistema_integrado.c common.c avl.c rubro_negra.c
```

No Windows com MinGW, use, quando disponível, `mingw32-make` no lugar de `make`. Também é possível compilar diretamente com `gcc`; os executáveis terão extensão `.exe`.

## 4.5 Rodar todos os testes

**Linux/macOS/MSYS2:**

```bash
./testes
./testes_integracao
./sistema_integrado --buscar "The Last Command"
./sistema_integrado --buscar "Gone with the Wind"
```

**Windows PowerShell, com os `.exe` compilados:**

```powershell
.\testes.exe
.\testes_integracao.exe
.\sistema_integrado.exe --buscar "The Last Command"
.\sistema_integrado.exe --buscar "Gone with the Wind"
```

Para consultar outro filme, substitua o argumento de `--buscar` pelo título desejado. Se os CSVs não estiverem em `dados/`, o programa imprimirá erro de abertura ou retornará conjunto vazio.

## 4.6 Como apresentar ao professor

1. Abra `README.md` e explique o domínio, a divisão de responsabilidades e o escopo da base.
2. Mostre `avl.h`/`avl.c`: nó com altura, inserção, rotações e busca.
3. Mostre `rubro_negra.h`/`rubro_negra.c`: cor, sentinela `NIL`, correção da inserção, rotações e busca.
4. Execute `./testes` e destaque as **50.000 chaves por árvore** nos testes sintéticos e as alturas 16 e 29.
5. Execute `./testes_integracao` e explique a soma de **16.001 + 35.000 = 51.001 registros simulados**.
6. Execute `./sistema_integrado --buscar "The Last Command"` e mostre as indicações e os dados de filme encontrados.
7. Termine explicando a diferença entre **teste com dados sintéticos** e **coleta histórica real**, além das operações pendentes.

### Fala sugerida para a demonstração

> “O projeto utiliza duas árvores binárias de busca autobalanceadas, agora implementadas em linguagem C. A AVL indexa indicações a premiações e a rubro-negra indexa um catálogo de filmes. Nosso objetivo é organizar e recuperar dados de fontes complementares. Nos testes estruturais, cada árvore recebeu 50 mil chaves simuladas, e também executamos uma integração de 51.001 registros simulados. Com os CSVs de amostra, a consulta por título retorna tanto as indicações quanto os dados do filme. A coleta de uma base histórica real com mais de 50 mil linhas ainda precisa ser finalizada e validada.”

---

# 5. Limitações, correções e trabalhos futuros

## 5.1 Correções aplicadas à versão C revisada

1. Corrigida a constante de temporização de `CLOCKSPERSEC` para **`CLOCKS_PER_SEC`**, necessária para compilar `sistema_integrado.c`.
2. Limitado o tamanho da chave produzida por `normalizar_string()` a `MAX_STR - 1`.
3. Corrigida a propagação de erro entre subárvores na rotina de validação da AVL.
4. Adicionada a leitura do campo `status_genero` quando presente no CSV de indicações.

## 5.2 Pendências técnicas conhecidas

- Implementar **remoção** em AVL e rubro-negra, com os respectivos ajustes de balanceamento.
- Implementar **percursos públicos** pré-ordem, em ordem e pós-ordem.
- Melhorar os validadores para conferir também **ordenação BST**, ponteiros para pais, contagens internas e outras invariantes.
- Substituir ou ampliar o parser CSV para suportar **aspas duplicadas**, campos com quebras de linha e registros longos.
- Tratar adequadamente falhas de `malloc()`, `realloc()` e outras condições de erro.
- Melhorar o tratamento de títulos Unicode/acentuados.
- Realizar associação confiável por **`imdb_id`**, quando esse identificador estiver disponível nas fontes.
- Obter e validar **50 mil ou mais linhas históricas e de catálogo reais**, registrando contagem, procedência e duplicidades; ampliar com BAFTA e Critics' Choice somente após verificar as fontes.
- Desenvolver benchmarks repetíveis de tempo e memória com bases reais de diferentes tamanhos.

---

# 6. Referências

1. ACADEMY OF MOTION PICTURE ARTS AND SCIENCES. *Academy Awards Database*. https://awardsdatabase.oscars.org/
2. GOLDEN GLOBES. *Awards Database*. https://goldenglobes.com/awards-database/
3. IMDb. *Non-Commercial Datasets*. https://developer.imdb.com/non-commercial-datasets/
4. GitHub. *Sidodj/SQL-Oscar-project*. https://github.com/Sidodj/SQL-Oscar-project
5. GitHub. *prasertcbs/basic-dataset*. https://github.com/prasertcbs/basic-dataset
6. CORMEN, Thomas H. et al. *Introduction to Algorithms*. 4. ed. MIT Press, 2022.
7. ISO/IEC. *ISO/IEC 9899: Programming Languages — C* (referência à linguagem C).
8. Repositório do grupo: https://github.com/Guilui16/Dracarys/tree/main/cinema_project

---

**Estado da entrega:** implementação em C compilada e testada com sucesso nos cenários descritos; documentação das operações efetivamente disponíveis; expansão da base real, remoção, percursos públicos e testes de desempenho aprofundados **pendentes**.
