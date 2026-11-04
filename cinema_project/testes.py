"""Testes de integridade e igualdade das arvores sem instalar bibliotecas."""
import random
from avl import ArvoreAVL
from rubro_negra import ArvoreRubroNegra


def testar(classe, n=50000):
    arvore=classe()
    valores=list(range(n))
    random.Random(2026).shuffle(valores)
    for chave in valores:
        arvore.inserir(f'{chave:06d}',{'filme':f'Filme {chave}'})
    assert arvore.tamanho==n
    assert arvore.validar()
    for numero in [0,1,17,n//2,n-1]:
        assert arvore.buscar(f'{numero:06d}')[0]['filme']==f'Filme {numero}'
    assert arvore.buscar('inexistente')==[]
    arvore.inserir('000017',{'filme':'Registro duplicado'})
    assert len(arvore.buscar('000017'))==2
    assert arvore.tamanho==n
    assert arvore.validar()
    print(classe.__name__, 'OK,',n,'chaves, altura:',arvore.altura())

if __name__=='__main__':
    testar(ArvoreAVL)
    testar(ArvoreRubroNegra)
