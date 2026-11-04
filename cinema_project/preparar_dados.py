"""Coleta e normaliza registros reais de Oscar, Globo de Ouro e filmes IMDb.
Executar com internet: python preparar_dados.py
Nao transforma filme sem indicacao em indicado; nao inventa subgeneros.
"""
import csv
import gzip
import io
import re
import unicodedata
from collections import Counter
from pathlib import Path
from urllib.request import Request, urlopen

BASE = Path(__file__).resolve().parent
DADOS = BASE / 'dados'
DADOS.mkdir(exist_ok=True)
FONTES = {
 'Oscar': 'https://raw.githubusercontent.com/Sidodj/SQL-Oscar-project/main/data/the_oscar_award.csv',
 'Globo de Ouro': 'https://raw.githubusercontent.com/prasertcbs/basic-dataset/master/golden_globe_awards.csv',
 'IMDb': 'https://datasets.imdbws.com/title.basics.tsv.gz',
}
CAMPOS = ['id','premiacao','ano_cerimonia','ano_filme','categoria','indicado','filme','vencedor','genero','subgenero','imdb_id','fonte','status_genero']


def baixar(url, destino):
    if destino.exists() and destino.stat().st_size > 100:
        print('Usando arquivo existente:', destino)
        return
    print('Baixando:', url)
    requisicao = Request(url, headers={'User-Agent':'CinemaAcademicResearch/1.0'})
    with urlopen(requisicao, timeout=90) as resposta, destino.open('wb') as saida:
        while True:
            bloco = resposta.read(1024 * 1024)
            if not bloco:
                break
            saida.write(bloco)


def limpar(txt):
    return (txt or '').strip()


def numero(txt):
    try:
        return int(txt)
    except (ValueError, TypeError):
        return ''


def normalizar(txt):
    txt = unicodedata.normalize('NFKD', (txt or '').casefold())
    txt = ''.join(c for c in txt if not unicodedata.combining(c))
    return re.sub(r'[^a-z0-9]+', ' ', txt).strip()


def ler_premiacoes():
    registros = []
    for nome, arquivo in [('Oscar', 'oscar.csv'), ('Globo de Ouro', 'globos.csv')]:
        url = FONTES[nome]
        baixar(url, DADOS / arquivo)
        with (DADOS / arquivo).open(encoding='utf-8-sig', newline='') as f:
            for r in csv.DictReader(f):
                if nome == 'Oscar':
                    ano_cerimonia = numero(r.get('year_ceremony'))
                    indicado = limpar(r.get('name'))
                    venceu = r.get('winner', '')
                else:
                    ano_cerimonia = numero(r.get('year_award'))
                    indicado = limpar(r.get('nominee'))
                    venceu = r.get('win', '')
                titulo = limpar(r.get('film'))
                registros.append({
                    'id': len(registros)+1, 'premiacao': nome,
                    'ano_cerimonia': ano_cerimonia, 'ano_filme': numero(r.get('year_film')),
                    'categoria': limpar(r.get('category')), 'indicado': indicado,
                    'filme': titulo, 'vencedor': '1' if str(venceu).lower() == 'true' else '0' if str(venceu).lower() == 'false' else '',
                    'genero': '', 'subgenero': '', 'imdb_id': '',
                    'fonte': url, 'status_genero': 'nao_vinculado',
                })
    return registros


def carregar_catalogo_imdb(registros, minimo=35000):
    """Lê IMDb em streaming; coleta filmes reais e candidatos por titulo/ano.
    Faz vinculo SOMENTE quando titulo+ano resultam em identificacao unica.
    """
    destino = DADOS / 'title.basics.tsv.gz'
    baixar(FONTES['IMDb'], destino)
    procurados = set()
    for r in registros:
        if r['filme']:
            y = r['ano_filme']
            if isinstance(y, int):
                for ano in [y - 1, y, y + 1]:
                    procurados.add((normalizar(r['filme']), ano))
    candidatos = {}
    catalogo = []
    contagem_filmes = 0
    with gzip.open(destino, 'rt', encoding='utf-8', newline='') as f:
        for r in csv.DictReader(f, delimiter='\t'):
            if r['titleType'] != 'movie' or r['isAdult'] != '0':
                continue
            ano = numero(r['startYear'])
            if not isinstance(ano, int):
                continue
            contagem_filmes += 1
            generos = '' if r['genres'] == r'\N' else r['genres']
            filme = {'imdb_id':r['tconst'],'filme':r['primaryTitle'], 'ano_filme':ano,
                     'genero':generos.split(',')[0] if generos else '',
                     'generos_imdb': generos, 'subgenero':'', 'fonte':FONTES['IMDb']}
            if len(catalogo) < minimo:
                catalogo.append(filme)
            # Indexa candidatos para vinculacao com premiacoes
            for titulo in (r['primaryTitle'], r['originalTitle']):
                chave = (normalizar(titulo), ano)
                if chave in procurados:
                    candidatos.setdefault(chave, {})[r['tconst']] = filme
    # Nunca inferir subgenero a partir do segundo genero IMDb.
    vinculados = 0
    ambiguos = 0
    for r in registros:
        if not r['filme'] or not isinstance(r['ano_filme'], int):
            continue
        # Prioriza ano exato, depois +/- 1, mas apenas resultados univocos.
        for distancia in (0, -1, 1):
            lista = candidatos.get((normalizar(r['filme']),r['ano_filme']+distancia),{})
            if len(lista) == 1:
                item = next(iter(lista.values()))
                r['imdb_id'] = item['imdb_id']
                r['genero'] = item['generos_imdb']
                r['status_genero'] = 'imdb_exato' if distancia == 0 else 'imdb_ano_aproximado_revisar'
                vinculados += 1
                break
            if len(lista) > 1:
                ambiguos += 1
                r['status_genero'] = 'ambiguidade_revisar'
                break
    return catalogo, contagem_filmes, vinculados, ambiguos


def salvar_csv(caminho, campos, registros):
    with caminho.open('w',encoding='utf-8-sig',newline='') as f:
        writer = csv.DictWriter(f,fieldnames=campos,extrasaction='ignore')
        writer.writeheader()
        writer.writerows(registros)


def main():
    registros = ler_premiacoes()
    print('Indicacoes observadas (Oscar+Globo):',len(registros))
    # Minimo combinado: 50.001 linhas; catalogo tem pelo menos 35 mil filmes.
    alvo_filmes = max(35000, 50001 - len(registros))
    catalogo, total, vinculados, ambiguos = carregar_catalogo_imdb(registros, minimo=alvo_filmes)
    assert len(registros) + len(catalogo) >= 50001, 'Nao foi atingido o minimo combinado de 50.001'
    assert len({r['imdb_id'] for r in catalogo}) == len(catalogo), 'IMDb IDs repetidos no catalogo'
    salvar_csv(DADOS/'indicacoes_reais.csv',CAMPOS,registros)
    salvar_csv(DADOS/'catalogo_filmes.csv',['imdb_id','filme','ano_filme','genero','generos_imdb','subgenero','fonte'],catalogo)
    with (DADOS/'resumo.txt').open('w',encoding='utf-8') as f:
        f.write(f'AVL (indicacoes Oscar e Globo): {len(registros)}\nRubro-negra (filmes reais IMDb): {len(catalogo)}\nSOMA: {len(registros)+len(catalogo)} registros distintos por tipo\nTotal IMDb lido: {total}\nIndicacoes vinculadas a IMDb: {vinculados}\nVinculos ambiguos: {ambiguos}\n')
        f.write('ATENCAO: SOMA de indicacoes e filmes NAO significa total de indicacoes.\n')
        f.write('Subgenero nao e fornecido diretamente pelo IMDb; coluna intencionalmente vazia.\n')
    print((DADOS/'resumo.txt').read_text(encoding='utf-8'))

if __name__ == '__main__':
    main()
