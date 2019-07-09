#using GA solve knapsack problem

### Problem:
* 有 10 種不同的物品 A~J，每種物品各有10個
* 背包最大容量 275

| Item  | A | B | C | D | E | F | G | H | I | J |
| ----- |---|---|---|---|---|---|---|---|---|---|
| Weight| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |
| Value | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |


已知最佳解為

A~F 各取10個
G 取 8個
I 取 1個

Total weight = 275 and value = 620


### Encode

A. 以二進位編碼，共 102 個bit，其中 
 *  [0] -  [9] for 'A' which weight = 1 and value = 6
 * [10] - [19] for 'B' which weight = 2 and value = 7
 * ......
 * ......
 * ......
 * [90] - [99] for 'J' which weight = 10 and value = 15
 * [100] for weight
 * [101] for fitness
 * 編碼範圍 ∈ [0,1]
 
B. 以整數編碼，共 10 個 bit，其中
 * [0] -  [9] for 'A' to 'J'
 * 編碼範圍 ∈ [0,10]
 
 
### Fitness

### Selection

### Crossover

### Mutation


### Argument
經反覆測試，較佳參數如下：

#### Type A
_**Tournament Selection + Single-Point Crossover + Multiple-Point Mutation**_

Argument as below:
```
Round: 100
Generation: 4000
Population: 4
Crossover rate: 50%
Mutation rate: 50%
Punishment coefficient: 18
``` 

Result as below:
```

Overweight count | Overweight rate | Best case weight | Avg. value | Best value | Best fitness
                0 |          0.00 % |              275 |     600.20 |        615 |          615
Best case: A: 10  B: 9  C: 10  D: 9  E: 10  F: 10  G: 9  H: 1  I: 0  J: 0  
Time taken: 26.40s
```


Punishment coefficient ALPHA = 18 時，overweight 的機率極低（1000 rounds 未出現一次）


  