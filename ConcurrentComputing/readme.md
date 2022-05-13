# arrayminmax.c (time measurement: sequential vs. concurrent)
  ## N = 10⁵
    - threads = 1: acel = 0.000977/0.000977 = 1.000000
    - threads = 2: acel = 0.000977/0.00977 = 1.000000
    - threads = 4: acel = 0.000977/0.000977 = 1.000000
  ## N = 10⁷
    - threads = 1: acel = 0.037109/0.039062 = 0.950000
    - threads = 2: acel = 0.036133/0.019531 = 1.850000
    - threads = 4: acel = 0.036133/0.019531 = 1.850000
  ## N = 10⁸
    - threads = 1: acel = 0.359375/0.383789 = 0.936387
    - threads = 2: acel = 0.359375/0.194336 = 1.849246
    - threads = 4: acel = 0.359375/0.188477 = 1.906736

# primes.c

  ## 10⁵ elementos
    ### 1 thread
    - Tempo sequencial:  0.074204
    - Tempo concorrente:  0.065762
    - Aceleração: 1.128364
    - Sequencial = Concorrente
    ### 2 threads
    - Tempo sequencial:  0.074604
    - Tempo concorrente:  0.036428
    - Aceleração: 2.047976
    - Sequencial = Concorrente
    ### 4 threads
    - Tempo sequencial:  0.075997
    - Tempo concorrente:  0.031963
    - Aceleração: 2.377646
    - Sequencial = Concorrente


## 10⁷ elementos
### 1 thread
- Tempo sequencial:  38.674132
- Tempo concorrente:  38.872486
- Aceleração: 0.994897
- Sequencial = Concorrente
### 2 threads
- Tempo sequencial:  38.833987
- Tempo concorrente:  19.661596
- Aceleração: 1.975119
- Sequencial = Concorrente
### 4 threads
- Tempo sequencial:  38.656033
- Tempo concorrente:  15.917554
- Aceleração: 2.428516
- Sequencial = Concorrente


## 10⁸ elementos
### 1 thread
- Tempo sequencial:  1022.013959
- Tempo concorrente:  1025.939979
- Aceleração: 0.996173
- Sequencial = Concorrente
### 2 threads
- Tempo sequencial:  1021.260903
- Tempo concorrente:  515.435439
- Aceleração: 1.981356
- Sequencial = Concorrente
### 4 threads
- Tempo sequencial:  1021.260903
- Tempo concorrente:  515.435439
- Aceleração: 1.981356
- Sequencial = Concorrente
