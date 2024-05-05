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

# Todo's e dúvidas!

- [X] **Pode usar omp_wget_time?**
    
    Sim, mas ainda é necessário considerar a saída do `time`.

- [X] **O input do usuário para escolher uma equação do sistema é feito como?**

    Quarto argumento na hora de rodar o programa: `./jacobipar N T seed Eq`

- [X] **É para tratar input inválido?**

    Não, código apenas considera inputs convenientes. 

- [X] **Como garantir diagonal dominante?**

    Soma todos os elementos da linha em módulo, exceto o que constitui a diagonal, soma 1 e joga no elemento da diagonal daquela linha.

- [X] **Precisa paralelizar entrada?**

    Não precisa!

- [ ] **Qual ordem de matriz é pra ser executada em tempo razoável? (Qual o número máximo que na prática o professor vai testar ?)**
    
    Indefinido...

- [ ] **O speedup do meu código tá razoavel?**

    Não, tem algo errado...

- [ ] **Qual a limitação do rand()?**

   Indefinido...

# EM MANUTENÇÃO...

## Alguns argumentos extras que adicionei

```./build/jacobipar N T seed Eq randLimit```

N -> tamanho da matriz (obrigatório)

T -> Threads (obrigatório)

seed -> seed do rand (obrigatório)

Eq -> equação para ver o valor (0 por padrão e n faz diferença na perfomance)

randLimit -> Valor de limite do rand(por padrão é 10)

- 0: positivos ilimitados (DEMORA MTO, MESMO PARA MATRIZES MTO PEQUENAS, nem consegui testar).

- -1:"sem limitar" permitindo valores positivos e negativos (vai beeeem rápido, para qualquer matriz, mas sem speedup quase nenhum).

- valores >0: positivos limitados ao valor (Pode ser rápido ou lento dependendo do limite, 100 já começa a ficar bem lento), teve o melhor speedup mas ainda é um speedup super ruim.

## tem 2 branches "atuais"

main -> código com o paralelismo full meu

trying-to-improve-parallelism -> código com todas as sugestões do paulo sérgio

provavelmente o sweet spot é algo entre essas 2, mas eu fiz vários intermediando entre o meu e o do paulo e a grosso modo n tem  melhora significativa
