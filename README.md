#using GA solve knapsack problem

##### Problem:
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


####編碼

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
 