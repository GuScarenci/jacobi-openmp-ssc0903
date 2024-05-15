# jacobi-openmp-ssc0903
Trabalho 1 para a Disciplina SSC0903 - Computação de Alto Desempenho

# Executando o código
Para compilar o código digite:
```bash
make
```

Para rodar o código e exibir a saída tanto do método sequencial quanto do paralelo digite:
```bash
make run ARGS="<N> <T> <S> <E>"
```

Para rodar somente o código sequencial digite:
```bash
make run_seq ARGS="<N> <T> <S> <E>"
```

Para rodar somente o código paralelo digite:
```bash
make run_par ARGS="<N> <T> <S> <E>"
```

Onde `N` é o tamanho da matriz, `T` é o número de threads, `S` é o número de iterações e `E` é a equação do sistema que o usuário deseja escolher para associar os resultados obtidos.

# Gerando dados
Para gerar dados criamos um script em Python que gera um arquivo CSV com os dados de tempo de execução do código sequencial e paralelo. Para rodar o script digite:
```bash
python scripts/daq.py <R>
```
Onde `R` é o número de vezes que o código será executado.

A saída do script será um arquivo `res/data.csv` com a média, mediana, desvio padrão dos tempos de execução do algoritmo sequencial e do paralelo e o speedup e a eficiência para cada par de carga trabalho (`N`) e número de threads (`T`).

# Autores
- [Beatriz Lomes](b.lomes@usp.br) - 12548038
- [Gustavo Scarenci](github.com/GuScarenci) - 12547792
- [Hugo Nakamura](https://github.com/ikuyorih9) - 12732037
- [Matheus Cirillo](github.com/cirillom) - 12547750
