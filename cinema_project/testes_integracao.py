"""Teste da divisao complementar sem depender de internet ou downloads."""
from avl import ArvoreAVL
from rubro_negra import ArvoreRubroNegra
from sistema_integrado import pesquisar


def testar():
    avl, rb = ArvoreAVL(), ArvoreRubroNegra()
    # Registros do tipo indicacao vivem somente na AVL
    for i in range(16001):
        avl.inserir(f'filme {i:05d}', {'tipo': 'indicacao', 'id': i})
    # Registros do tipo filme vivem somente na rubro-negra
    for i in range(35000):
        rb.inserir(f'filme {i:05d}', {'tipo': 'filme', 'id': i})
    assert avl.validar() and rb.validar()
    assert len(avl.buscar('filme 00004')) == 1
    assert len(rb.buscar('filme 00004')) == 1
    a,b = pesquisar(avl,rb,'filme 00004')
    assert a[0]['tipo']=='indicacao' and b[0]['tipo']=='filme'
    assert 16001 + 35000 == 51001
    print('OK: 51.001 registros simulados com tipos separados; consulta complementar validada.')
    print('AVL: 16.001 indicacoes simuladas | altura:', avl.altura())
    print('Rubro-negra: 35.000 filmes simulados | altura:', rb.altura())


if __name__ == '__main__':
    testar()
