"""Arvore AVL: cada no armazena uma chave e TODOS os registros dessa chave.
Uso: python avl.py --arquivo filmes --campo genero --buscar Drama
"""
import argparse
from comum import carregar, demonstrar, CAMINHO_FILMES, CAMINHO_INDICACOES

class No:
    def __init__(self, chave, registro):
        self.chave = chave
        self.registros = [registro]
        self.esquerda = None
        self.direita = None
        self.altura = 1

class ArvoreAVL:
    def __init__(self):
        self.raiz = None
        self.tamanho = 0

    def _altura(self, no):
        return no.altura if no else 0

    def _atualizar(self, no):
        no.altura = 1 + max(self._altura(no.esquerda), self._altura(no.direita))

    def _balanceamento(self, no):
        return self._altura(no.esquerda) - self._altura(no.direita)

    def _girar_direita(self, y):
        x = y.esquerda
        y.esquerda = x.direita
        x.direita = y
        self._atualizar(y)
        self._atualizar(x)
        return x

    def _girar_esquerda(self, x):
        y = x.direita
        x.direita = y.esquerda
        y.esquerda = x
        self._atualizar(x)
        self._atualizar(y)
        return y

    def _inserir(self, no, chave, registro):
        if no is None:
            self.tamanho += 1
            return No(chave, registro)
        if chave == no.chave:
            no.registros.append(registro)
            return no
        if chave < no.chave:
            no.esquerda = self._inserir(no.esquerda, chave, registro)
        else:
            no.direita = self._inserir(no.direita, chave, registro)
        self._atualizar(no)
        bal = self._balanceamento(no)
        if bal > 1:
            if chave > no.esquerda.chave:
                no.esquerda = self._girar_esquerda(no.esquerda)
            return self._girar_direita(no)
        if bal < -1:
            if chave < no.direita.chave:
                no.direita = self._girar_direita(no.direita)
            return self._girar_esquerda(no)
        return no

    def inserir(self, chave, registro):
        self.raiz = self._inserir(self.raiz, str(chave), registro)

    def buscar(self, chave):
        no = self.raiz
        chave = str(chave)
        while no:
            if chave == no.chave:
                return no.registros
            no = no.esquerda if chave < no.chave else no.direita
        return []

    def altura(self):
        return self._altura(self.raiz)

    def validar(self):
        def conferir(no, inferior=None, superior=None):
            if no is None:
                return 0
            assert (inferior is None or no.chave > inferior)
            assert (superior is None or no.chave < superior)
            esquerda = conferir(no.esquerda,inferior,no.chave)
            direita = conferir(no.direita,no.chave,superior)
            assert abs(esquerda-direita)<=1, 'AVL desbalanceada'
            assert no.altura == max(esquerda,direita)+1
            return no.altura
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
    arvore=ArvoreAVL()
    demonstrar(arvore,registros,args.campo,args.buscar)
    print('Integridade AVL:',arvore.validar())
