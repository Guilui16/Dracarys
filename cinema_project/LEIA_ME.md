# Projeto revisado — Arvores complementares para cinema

## Decisao de arquitetura

Duas arvores que **se complementam**, em vez de receberem as mesmas linhas:

- **AVL = indicacoes e vitorias** de Oscar e Globo de Ouro, provenientes dos CSV historicos pesquisados. Uma linha e uma indicacao em uma categoria/ano. Indexa pelo nome normalizado do filme, ou pelo indicado quando o titulo nao aparece na fonte.
- **Rubro-negra = catalogo de filmes** IMDb, com titulos, ano e generos. Uma linha e um filme com identificador IMDb. Indexa pelo titulo normalizado do filme.
- **sistema_integrado.py** consulta os dois indices para exibir tanto premios quanto informacoes do catalogo. Uma associacao confiavel entre os dois conjuntos depende de identificador IMDb verificado; titulo igual, sozinho, nao comprova identidade.

## Quantidade de dados

Ao executar `preparar_dados.py` com internet, coletamos TODOS os registros recebidos das duas fontes de premios e pelo menos **35.000 filmes reais** do IMDb. Se o total ficar abaixo de 50.001, o coletor aumenta automaticamente a quantidade de filmes de catalogo ate atingir **no minimo 50.001 linhas somando os dois conjuntos**. A quantidade exata fica em `dados/resumo.txt`. Isso e um *total misto* — nao equivale a 50 mil indicacoes.

**Sem downloads, os 50.001 registros nao estao contidos neste ZIP.** Sao necessarios rede e acesso aos arquivos de origem. O projeto traz cinco linhas de amostra apenas para conferir formato e nao fabrica dados.

## Como executar (Python 3.10+)

Na pasta `cinema_project`, conectado a internet:

```sh
python preparar_dados.py
python testes.py
python testes_integracao.py
python sistema_integrado.py --buscar "The Last Command"
```

Os resultados sao:

- `dados/indicacoes_reais.csv`: alimenta **somente AVL**.
- `dados/catalogo_filmes.csv`: alimenta **somente rubro-negra**.
- `dados/resumo.txt`: mostra quantidades, total e vinculos.

## O que acontece em cada parte

1. `preparar_dados.py` baixa Oscar, Globos de Ouro e IMDb; uniformiza colunas e identifica genero quando titulo/ano permitem associacao suficientemente determinada.
2. `avl.py` guarda indicacoes, faz rotacoes para manter a diferenca de alturas das subarvores limitada a um, e permite `buscar(chave)`.
3. `rubro_negra.py` guarda filmes, mantem regras de cores (raiz preta, nenhum vermelho com filho vermelho e mesma altura negra dos caminhos) e permite `buscar(chave)`.
4. `sistema_integrado.py` le os dois arquivos, monta a AVL e a rubro-negra, imprime tamanho, altura, tempo de montagem e devolve resultados de ambas pela mesma consulta de titulo.
5. `testes.py` testa 50 mil **chaves sinteticas de teste de algoritmo**. `testes_integracao.py` testa que 16.001 indicacoes ficticias e 35.000 filmes ficticios em estruturas distintas totalizam 51.001 registros. Esses testes NAO sao bases reais.

## Exemplo ilustrativo de complementaridade

Uma busca por `The Last Command` pode recuperar na AVL uma indicacao em uma categoria e, na rubro-negra, o cadastro IMDb do filme. Os dados nao precisam ser duplicados; cada arvore informa coisas diferentes.

## Cuidados de rigor

- O IMDb tem **generos**, mas nao uma coluna oficial padronizada de **subgeneros**: `subgenero` e vazio quando nao documentado.
- Oscar + Globo de Ouro sao os premios incluidos atualmente. BAFTA e Critics Choice ainda NAO foram integrados.
- 50 mil registros mistos NAO sao 50 mil filmes premiados nem 50 mil indicacoes.
- Uma linha de filme pode corresponder a varias linhas de indicacao. Somar os tamanhos mede quantidade de *registros*, nao entidades cinematograficas unicas.
- A amostragem IMDb usa os primeiros filmes elegiveis do dump, sem pretensao de representatividade estatistica.
- A fonte IMDb deve ser usada respeitando as condicoes de uso pessoal/nao comercial.
- AVL e rubro-negra sao arvores de **busca balanceadas**, nao arvores de decisao de aprendizado de maquina.

## Fontes

- Oscar: https://raw.githubusercontent.com/Sidodj/SQL-Oscar-project/main/data/the_oscar_award.csv
- Globo: https://raw.githubusercontent.com/prasertcbs/basic-dataset/master/golden_globe_awards.csv
- IMDb: https://developer.imdb.com/non-commercial-datasets/
- BAFTA (a incorporar): https://www.bafta.org/awards/search/
- Critics Choice (a incorporar): https://www.criticschoice.com/critics-choice-awards-archives/
