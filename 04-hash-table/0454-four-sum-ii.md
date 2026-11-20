# [0454] 四數相加 II (4Sum II)

- **難度**：Medium
- **標籤**：`Array` `Hash Table`
- **刷題日期**：2026-11-20
- **複習狀態**：🟢 分組雜湊最佳解與常數優化皆已掌握

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**: 給定四個長度皆為 $n$ 的整數陣列 `nums1`、`nums2`、`nums3`、`nums4`，計算有多少組索引四元組 $(i, j, k, l)$ 滿足：
  1. $0 \le i, j, k, l \lt n$
  2. $\text{nums1}[i] + \text{nums2}[j] + \text{nums3}[k] + \text{nums4}[l] = 0$
* **關鍵規格差異（與 LC 1 Two Sum、LC 15 3Sum 對比）** :
  - 本題四個數字分別來自**四個獨立陣列**，且題目僅要求回傳**合法索引組合的總個數**（`int`），不需回傳具體索引或數值組合，也**不需要去重**（即使不同索引的數值相同，仍視為不同的合法四元組）。
* **輸入限制**:
  - $n == \text{nums1.length} == \text{nums2.length} == \text{nums3.length} == \text{nums4.length}$
  - $1 \le n \le 200$
  - $-2^{28} \le \text{nums1}[i], \text{nums2}[i], \text{nums3}[i], \text{nums4}[i] \le 2^{28}$
* **特殊狀況 (Edge Cases)** :
  - **大數相加溢位檢查**: 單一元素最大為 $2^{28}$，四個元素相加最大為 $4 \times 2^{28} = 2^{30}$，仍在 32 位元有號整數（`int` 上限 $2^{31}-1$）安全範圍內，不會發生整數溢位。
  - **全零陣列最壞情況**: 當四個陣列所有元素皆為 $0$ 且 $n = 200$ 時，總組合數為 $200^4 = 1.6 \times 10^9$，同樣在 `int` 可容納範圍內。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🔴 思路一：暴力四層迴圈 $\rightarrow$ 三層迴圈＋單元素雜湊
* **四層枚舉 ($O(n^4)$)** : 直接枚舉所有 $(i, j, k, l)$，當 $n = 200$ 時運算量達 $1.6 \times 10^9$，必定 TLE。
* **單元素雜湊 ($O(n^3)$)** : 將等式改寫為 $\text{nums4}[l] = -(\text{nums1}[i] + \text{nums2}[j] + \text{nums3}[k])$，先將 `nums4` 存入雜湊表再跑三層迴圈，時間降至 $O(n^3)$（約 $8 \times 10^6$），但仍不夠快。

### 🟢 最佳化解法：分組雜湊法 (Meet-in-the-Middle / 2 + 2 Grouping)
* **核心思想**: 將四個陣列平均拆成兩組（前兩個一組、後兩個一組），把四數之和拆解為「兩組兩數之和互為相反數」：
  $$\text{nums1}[i] + \text{nums2}[j] = -(\text{nums3}[k] + \text{nums4}[l])$$
* **為何只需建立「一個」`unordered_map`（而非兩個）？**
  - 不需為 `(nums3, nums4)` 額外建立第二張雜湊表。只需用單一張 `unordered_map<int, int> n1n2` 統計前半組 `a + b` 出現的**次數**，後半組直接雙層走訪 `c`、`d` 並查詢 `-(c + d)`，省去一半空間與一次建表常數開銷。
* **演算法步驟**:
  1. 初始化 `std::unordered_map<int, int> n1n2`，並呼叫 `n1n2.reserve(n * n)` 預先分配足夠桶子空間。
  2. **上半場**: 雙層走訪 `nums1` 與 `nums2`，將所有 `a + b` 的出現次數累加進 `n1n2[a + b]++`（`Key` 為兩數之和，`Value` 為湊出該和的索引組合數）。
  3. **下半場**: 雙層走訪 `nums3` 與 `nums4`，計算目標互補值 `-(c + d)`；若在 `n1n2` 中找到該鍵值，將對應的組合次數 `it->second` 累加至總計數器 `count`。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**: $O(n^2)$ — 前半段雙層迴圈走訪 `nums1` 與 `nums2` 花費 $O(n^2)$，後半段雙層迴圈走訪 `nums3` 與 `nums4` 並進行平均 $O(1)$ 的雜湊表查詢同樣花費 $O(n^2)$，總時間複雜度為 $O(n^2)$。
* **空間複雜度**: $O(n^2)$ — 最壞情況下（`nums1[i] + nums2[j]` 的 $n^2$ 種組合之和互不相同），`unordered_map` 需儲存 $n^2$ 組鍵值對。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **題目目標決定 `Value` 存什麼**: 與 LC 1（Two Sum）需回傳具體索引不同，本題只問「總共有幾組四元組」，因此 `Value` 直接存**出現次數（Count）**，查到時直接 `count += it->second` 即可，不需記錄具體索引清單。
* **預分配雜湊桶避免動態擴容 (`reserve`)** : 最壞情況下鍵值對數量可達 $n^2$（最大 $40,000$ 筆），在插入前先執行 `n1n2.reserve(n * n)` 可避免 `unordered_map` 在運行途中反覆觸發耗時的擴容與 Rehash，大幅提升常數效能。
* **善用 Range-based `for` 迴圈降低手殘風險**: 由於本題計算過程完全不需要用到陣列下標 `i`、`j`，改用 `for (int a : nums1)` 與 `for (int b : nums2)` 不僅語法更簡潔，也能徹底避免雙層迴圈中把 `nums4[j]` 誤打成 `nums4[i]` 這類極難肉眼 Debug 的索引混淆錯誤。

---

## 5. 程式碼實作 (C++)

```cpp
class Solution {
public:
    int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
        int n = nums1.size();
        // Key: nums1[i] + nums2[j] 的和, Value: 該和出現的次數
        std::unordered_map<int, int> n1n2;
        n1n2.reserve(n * n); // 預先分配足夠桶子空間，避免動態擴容與 rehash

        // (nums1, nums2) 所有兩兩相加之和的出現次數
        for (int a : nums1) {
            for (int b : nums2) {
                n1n2[a + b]++;
            }
        }

        int count = 0;
        // (nums3, nums4)，尋找互補值 -(c + d)
        for (int c : nums3) {
            for (int d : nums4) {
                int complement = 0 - (c + d);
                if (auto it = n1n2.find(complement); it != n1n2.end()) {
                    count += it->second;
                }
            }
        }

        return count;
    }
};
```