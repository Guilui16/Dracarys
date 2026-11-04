"""Operacoes comuns: ler CSV, buscar e relatar resultados."""
import csv
import time
from pathlib import Path

PASTA = Path(__file__).resolve().parent
CAMINHO_FILMES = PASTA/'dados'/'catalogo_filmes.csv'
CAMINHO_INDICACOES = PASTA/'dados'/'indicacoes_reais.csv'


def carregar(caminho):
    with open(caminho, newline='', encoding='utf-8-sig') as arquivo:
        return list(csv.DictReader(arquivo))


def demonstrar(arvore, registros, campo, valor):
    inicio = time.perf_counter()
    for r in registros:
        arvore.inserir(r[campo], r)
    carga = time.perf_counter()-inicio
    inicio = time.perf_counter()
    encontrados = arvore.buscar(valor)
    busca = time.perf_counter()-inicio
    print(f'Registros: {len(registros)} | Chaves distintas: {arvore.tamanho}')
    print(f'Altura da arvore: {arvore.altura()}')
    print(f'Tempo de insercao: {carga:.6f}s | busca: {busca:.8f}s')
    print(f'Pesquisa por {campo}={valor!r}: {len(encontrados)} resultados')
    for r in encontrados[:8]:
        print('  ',r.get('filme',''),r.get('ano_filme',''),r.get('genero',''),r.get('premiacao',''),r.get('categoria',''))
    return encontrados
