CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = common.c avl.c rubro_negra.c

all: sistema_integrado testes testes_integracao

sistema_integrado: sistema_integrado.c $(SRCS)
	$(CC) $(CFLAGS) -o sistema_integrado sistema_integrado.c $(SRCS)

testes: testes.c $(SRCS)
	$(CC) $(CFLAGS) -o testes testes.c $(SRCS)

testes_integracao: testes_integracao.c $(SRCS)
	$(CC) $(CFLAGS) -o testes_integracao testes_integracao.c $(SRCS)

clean:
	rm -f sistema_integrado testes testes_integracao
