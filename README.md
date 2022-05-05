# Uni_Proj
## ConcComp:
- arrayminmax.c (time measurement: sequential vs. concurrent)
  - N = 10⁵
    - threads = 1: acel = 0.000977/0.000977 = 1.000000
    - threads = 2: acel = 0.000977/0.00977 = 1.000000
    - threads = 4: acel = 0.000977/0.000977 = 1.000000
  - N = 10⁷
    - threads = 1: acel = 0.037109/0.039062 = 0.950000
    - threads = 2: acel = 0.036133/0.019531 = 1.850000
    - threads = 4: acel = 0.036133/0.019531 = 1.850000
  - N = 10⁸
    - threads = 1: acel = 0.359375/0.383789 = 0.936387
    - threads = 2: acel = 0.359375/0.194336 = 1.849246
    - threads = 4: acel = 0.359375/0.188477 = 1.906736
