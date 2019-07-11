#using GA solve knapsack problem

### Problem:
* 有 10 種不同的物品 A~J，每種物品各有10個
* 背包最大容量 275

| Item  | A | B | C | D | E | F | G | H | I | J |
| ----- |---|---|---|---|---|---|---|---|---|---|
| Weight| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |
| Value | 6 | 7 | 8 | 9 |10 |11 |12 |13 |14 |15 |


已知最佳解為

Total weight = 275 and value = 620

有多種取法，例如：A~F 各取10個、G 取 8個、I 取 1個




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
 * weight、value 改存在 struct 中
 
### Fitness

一般而言 `fitness = weight * value`

但當背包超重時則需要一個懲罰係數來搭配，例如：

`fitness = value - ALPHA * (weight - KNAPSACK_SIZE);`

經過反覆測試，`ALPHA = 18` 時，極少出現超重情形（至少 2000 rounds 以上都沒出現過一次超重）。因此採用 `ALPHA = 18`


### Selection

#### Tournament Selection

1. 從母群中隨機挑選兩條染色體來比較，將適應值較高者放入交配池中。
2. 重複 step 1 直到交配池數量等於母群。

#### Roulette Selection

### Crossover

### Mutation


### Argument
經反覆測試，目前為止最佳的參數如下：

```
Selection type: Tournament
Crossover type: Single-Point
Mutation type: Single-Point
Knapsack size: 275
Round: 50
Generation: 5000
Population: 16
Crossover rate: 50%
Mutation rate: 50%
Punishment coefficient: 18

 Overweight count | Overweight rate | Best case weight | Avg. value | Best value | Best fitness
                0 |          0.00 % |              275 |     610.62 |        620 |          620
Best case: A: 10  B: 10  C: 10  D: 10  E: 10  F: 9  G: 9  H: 1  I: 0  J: 0  
Time taken: 296.01s
```
