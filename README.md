# jacobi-openmp-ssc0903
Trabalho 1 para a Disciplina SSC0903 - Computação de Alto Desempenho

# Executando o código
Para compilar o código digite:
```bash
make
```

Para rodar o código e exibir a saída tanto do método sequencial quanto do paralelo digite:
```bash
make run ARGS="<N> <T> <S>"
```

Para rodar somente o código sequencial digite:
```bash
make run_seq ARGS="<N> <T> <S>"
```

Para rodar somente o código paralelo digite:
```bash
make run_par ARGS="<N> <T> <S>"
```

Onde `N` é o tamanho da matriz, `T` é o número de threads e `S` é o número de iterações.

# Autores
- [Beatriz Lomes](b.lomes@usp.br) - 12548038
- [Gustavo Scarenci](github.com/GuScarenci) - 12547792
- [Hugo Nakamura](https://github.com/ikuyorih9) - 12732037
- [Matheus Cirillo](github.com/cirillom) - 12547750