"""Consulta complementar: AVL=indicacoes; rubro-negra=catalogo de filmes.
Nao mistura os tipos de linhas nem conta uma indicacao como filme distinto.
Uso: python sistema_integrado.py --buscar Oppenheimer
"""
import argparse
import csv
from collections import Counter
from pathlib import Path
from time import perf_counter
from preparar_dados import normalizar
from avl import ArvoreAVL
from rubro_negra import ArvoreRubroNegra

DADOS = Path(__file__).resolve().parent / 'dados'


def ler(nome):
    with (DADOS / nome).open(newline='', encoding='utf-8-sig') as arquivo:
        return list(csv.DictReader(arquivo))


def montar():
    indicacoes = ler('indicacoes_reais.csv')
    filmes = ler('catalogo_filmes.csv')
    assert len(indicacoes) + len(filmes) >= 50001, 'Base total inferior a 50.001 registros'
    assert len({r['imdb_id'] for r in filmes}) == len(filmes), 'Catalogo IMDb duplicado'
    avl, rn = ArvoreAVL(), ArvoreRubroNegra()
    inicio = perf_counter()
    for r in indicacoes:
        # Indicado substitui titulo ausente somente para que a linha seja pesquisavel
        chave = normalizar(r['filme'] or r['indicado'])
        avl.inserir(chave or f"sem titulo {r['id']}", r)
    tempo_avl = perf_counter() - inicio
    inicio = perf_counter()
    for r in filmes:
        rn.inserir(normalizar(r['filme']), r)
    tempo_rn = perf_counter() - inicio
    assert avl.validar() and rn.validar()
    return avl, rn, indicacoes, filmes, tempo_avl, tempo_rn


def pesquisar(avl, rn, titulo):
    chave = normalizar(titulo)
    return avl.buscar(chave), rn.buscar(chave)


def apresentar(avl, rn, indicacoes, filmes, tempo_avl, tempo_rn, titulo):
    print('=== INDICES COMPLEMENTARES ===')
    print(f'AVL: {len(indicacoes)} indicacoes | {avl.tamanho} chaves | altura {avl.altura()} | montagem {tempo_avl:.4f}s')
    print(f'Rubro-negra: {len(filmes)} filmes | {rn.tamanho} chaves | altura {rn.altura()} | montagem {tempo_rn:.4f}s')
    print(f'TOTAL COMBINADO: {len(indicacoes) + len(filmes)} linhas (tipos diferentes)')
    t0 = perf_counter()
    achados_indicacoes, achados_filmes = pesquisar(avl, rn, titulo)
    tempo = perf_counter() - t0
    print(f'\nBusca por titulo: {titulo!r} ({tempo:.8f}s)')
    print(f'Indicacoes: {len(achados_indicacoes)} | Itens de catalogo: {len(achados_filmes)}')
    for r in achados_indicacoes[:8]:
        print('  INDICACAO:', r['premiacao'], r['ano_cerimonia'], r['categoria'], 'vencedor='+r['vencedor'])
    for r in achados_filmes[:8]:
        print('  FILME:', r['filme'], r['ano_filme'], r['generos_imdb'], r['imdb_id'])
    # Vinculos somente por IMDb ID previamente verificado; nome e insuficiente.
    ids = {r['imdb_id'] for r in achados_filmes}
    ligadas = [r for r in achados_indicacoes if r['imdb_id'] and r['imdb_id'] in ids]
    print(f'Indicacoes ligadas aos filmes achados por IMDb ID: {len(ligadas)}')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--buscar', default='The Last Command', help='Titulo a consultar nas duas arvores')
    args = parser.parse_args()
    a, b, i, f, ta, tb = montar()
    apresentar(a, b, i, f, ta, tb, args.buscar)


if __name__ == '__main__':
    main()
