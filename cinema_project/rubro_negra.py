"""Arvore rubro-negra com balanceamento por recoloracao e rotacoes.
Uso: python rubro_negra.py --arquivo filmes --campo genero --buscar Drama
"""
import argparse
from comum import carregar, demonstrar, CAMINHO_FILMES, CAMINHO_INDICACOES

VERMELHO = 'vermelho'
PRETO = 'preto'

class No:
    def __init__(self, chave=None, registro=None, cor=PRETO):
        self.chave=chave
        self.registros=[] if registro is None else [registro]
        self.cor=cor
        self.pai=None
        self.esquerda=None
        self.direita=None

class ArvoreRubroNegra:
    def __init__(self):
        self.NIL=No()
        self.NIL.esquerda=self.NIL
        self.NIL.direita=self.NIL
        self.NIL.pai=self.NIL
        self.raiz=self.NIL
        self.tamanho=0

    def _girar_esquerda(self,x):
        y=x.direita
        x.direita=y.esquerda
        if y.esquerda is not self.NIL:
            y.esquerda.pai=x
        y.pai=x.pai
        if x.pai is self.NIL:
            self.raiz=y
        elif x is x.pai.esquerda:
            x.pai.esquerda=y
        else:
            x.pai.direita=y
        y.esquerda=x
        x.pai=y

    def _girar_direita(self,y):
        x=y.esquerda
        y.esquerda=x.direita
        if x.direita is not self.NIL:
            x.direita.pai=y
        x.pai=y.pai
        if y.pai is self.NIL:
            self.raiz=x
        elif y is y.pai.direita:
            y.pai.direita=x
        else:
            y.pai.esquerda=x
        x.direita=y
        y.pai=x

    def inserir(self,chave,registro):
        chave=str(chave)
        pai=self.NIL
        atual=self.raiz
        while atual is not self.NIL:
            pai=atual
            if chave==atual.chave:
                atual.registros.append(registro)
                return
            atual=atual.esquerda if chave<atual.chave else atual.direita
        novo=No(chave,registro,VERMELHO)
        novo.esquerda=novo.direita=self.NIL
        novo.pai=pai
        if pai is self.NIL:
            self.raiz=novo
        elif chave<pai.chave:
            pai.esquerda=novo
        else:
            pai.direita=novo
        self.tamanho+=1
        self._corrigir_insercao(novo)

    def _corrigir_insercao(self,z):
        while z.pai.cor==VERMELHO:
            if z.pai is z.pai.pai.esquerda:
                tio=z.pai.pai.direita
                if tio.cor==VERMELHO:
                    z.pai.cor=PRETO
                    tio.cor=PRETO
                    z.pai.pai.cor=VERMELHO
                    z=z.pai.pai
                else:
                    if z is z.pai.direita:
                        z=z.pai
                        self._girar_esquerda(z)
                    z.pai.cor=PRETO
                    z.pai.pai.cor=VERMELHO
                    self._girar_direita(z.pai.pai)
            else:
                tio=z.pai.pai.esquerda
                if tio.cor==VERMELHO:
                    z.pai.cor=PRETO
                    tio.cor=PRETO
                    z.pai.pai.cor=VERMELHO
                    z=z.pai.pai
                else:
                    if z is z.pai.esquerda:
                        z=z.pai
                        self._girar_direita(z)
                    z.pai.cor=PRETO
                    z.pai.pai.cor=VERMELHO
                    self._girar_esquerda(z.pai.pai)
        self.raiz.cor=PRETO

    def buscar(self,chave):
        atual=self.raiz
        chave=str(chave)
        while atual is not self.NIL:
            if chave==atual.chave:
                return atual.registros
            atual=atual.esquerda if chave<atual.chave else atual.direita
        return []

    def altura(self):
        def altura_no(no):
            if no is self.NIL:
                return 0
            return 1+max(altura_no(no.esquerda),altura_no(no.direita))
        return altura_no(self.raiz)

    def validar(self):
        assert self.raiz.cor==PRETO
        assert self.NIL.cor==PRETO
        def conferir(no, inferior=None,superior=None):
            if no is self.NIL:
                return 1
            assert (inferior is None or no.chave>inferior)
            assert (superior is None or no.chave<superior)
            if no.cor==VERMELHO:
                assert no.esquerda.cor==PRETO and no.direita.cor==PRETO
            if no.esquerda is not self.NIL:
                assert no.esquerda.pai is no
            if no.direita is not self.NIL:
                assert no.direita.pai is no
            a=conferir(no.esquerda,inferior,no.chave)
            b=conferir(no.direita,no.chave,superior)
            assert a==b, 'Altura negra diferente'
            return a + (1 if no.cor==PRETO else 0)
        conferir(self.raiz)
        return True

if __name__ == '__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--arquivo',choices=['filmes','indicacoes'],default='filmes')
    parser.add_argument('--campo',default='genero')
    parser.add_argument('--buscar',default='Drama')
    args=parser.parse_args()
    caminho=CAMINHO_FILMES if args.arquivo=='filmes' else CAMINHO_INDICACOES
    registros=carregar(caminho)
    if registros and args.campo not in registros[0]:
        parser.error('Campo nao existe no CSV')
    arvore=ArvoreRubroNegra()
    demonstrar(arvore,registros,args.campo,args.buscar)
    print('Integridade rubro-negra:',arvore.validar())
