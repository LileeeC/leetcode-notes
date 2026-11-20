# [0001] 兩數之和 (Two Sum)

- **難度**：Easy
- **標籤**：`Array` `Hash Table`
- **刷題日期**：2026-11-19
- **複習狀態**：🟢 One-pass Hash Map 最佳解與 Duplicate Follow-up 皆已掌握

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定一個整數陣列 `nums` 與一個目標整數 `target`，在陣列中找出和為 `target` 的兩個整數，並回傳它們的**陣列索引（Indices）**，回傳順序不限。
* **題目條件保證**：
  - 每組輸入只會對應**唯一一個**有效答案。
  - 陣列中**同一個元素（相同索引位置）不能重複使用兩次**（例如 `nums = [3, 2, 4]`, `target = 6` 時，不能回傳 `[0, 0]`）。
* **輸入限制**：
  - $2 \le \text{nums.length} \le 10^4$
  - $-10^9 \le \text{nums}[i] \le 10^9$
  - $-10^9 \le \text{target} \le 10^9$
* **特殊狀況 (Edge Cases)** ：
  - **負數與零**：元素與 `target` 皆可能為負數或 `0`（例如 `nums = [-3, 4, 3, 90]`, `target = 0`），因此無法使用固定範圍陣列作為直接定址表，必須使用動態雜湊表。
  - **數值重複但索引不同**：例如 `nums = [3, 3]`, `target = 6`，必須正確回傳 `[0, 1]`。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🔴 暴力雙層迴圈法 (Brute Force)
* **直覺**：使用兩層 `for` 迴圈枚舉所有索引配對 $(i, j)$（其中 $i \lt j$），檢查 `nums[i] + nums[j] == target`。
* **瓶頸**：時間複雜度為 $O(N^2)$，當陣列長度達 $10^4$ 時執行效率差。

### 🟢 最佳化解法：單次掃描雜湊表 (One-pass `std::unordered_map`)
* **為何從 `unordered_set` 升級為 `unordered_map`？**
  - `unordered_set` 僅能回答「元素是否存在」，無法保存元素原本的陣列位置。
  - 本題查詢到互補數 `complement = target - nums[i]` 存在後，還必須取回它的索引，因此需要建立 **「數值 $\rightarrow$ 索引」** 的鍵值映射（`std::unordered_map<int, int>`）。
* **演算法步驟（先查後存，避免自我配對）**：
  1. 初始化空的 `std::unordered_map<int, int> seen`（`Key` 存數字的值 `nums[i]`，`Value` 存該數字的索引 `i`）。
  2. 使用整數索引迴圈 `for (int i = 0; i < nums.size(); i++)` 走訪陣列：
     - 計算當前元素需要的互補數 `int complement = target - nums[i]`。
     - 透過 `auto it = seen.find(complement)` 在**先前已走訪過的元素**中查找是否存在該互補數。
     - 若 `it != seen.end()`，代表配對成功，直接回傳 `{it->second, i}`（其中 `it->second` 為互補數的索引）。
     - 若未找到，才將當前元素存入雜湊表：`seen[nums[i]] = i`，供後續元素配對使用。

### 🚀 面試 Follow-up：若輸入包含重複元素（Duplicates），複雜度是否會退化成 $O(N^2)$？
面試被問到 *"What if duplicates exist?"* 時，應先釐清**回傳規格**再分析複雜度：
1. **規格一：回傳任意一組解（同原題規格，如 `nums = [1, 1, 1, 1]`, `target = 2`）**
   - 時間複雜度依然是 **$O(N)$**（甚至更快，走到 `i = 1` 立刻匹配 `{0, 1}` 結束）。即便後方存在相同數值，直接覆蓋 `seen[nums[i]] = i` 也不影響單次線性掃描效率。
2. **規格二：回傳所有「不重複的數值配對」（Unique Value Pairs，如 `[[1, 1]]`）**
   - 時間複雜度依然是 **$O(N)$**（利用 `unordered_map<int, int>` 記錄數值出現次數或已配對狀態）或排序後雙指標 $O(N \log N)$。
3. **規格三：回傳所有「合法的索引配對」（All Index Pairs，如 `[0,1], [0,2], [0,3], [1,2], [1,3], [2,3]`）**
   - 此時時間複雜度為 **$\Theta(N^2)$**，且已是**理論最佳下界（Optimal）**。原因在於當 $N$ 個元素全相同且兩兩相加皆為 `target` 時，合法組合共有 $\binom{N}{2} = \frac{N(N-1)}{2}$ 組，**輸出結果本身的規模（Output-Sensitive）就是 $O(N^2)$**，任何演算法皆無法在低於 $O(N^2)$ 的時間內寫出所有結果。實作上可將雜湊表改為 `unordered_map<int, vector<int>>` 儲存同一數值的所有歷史索引清單。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**：$O(N)$ — 僅對長度為 $N$ 的陣列進行單次線性走訪（One-pass），每次雜湊表的 `.find()` 查詢與插入平均時間複雜度皆為 $O(1)$。
* **空間複雜度**：$O(N)$ — 最壞情況下（答案在最後兩個元素或無解），`unordered_map` 需儲存最多 $N$ 組鍵值對。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **「先查後存」的重要性**：切勿一開始就把整個 `nums` 全部塞進 `unordered_map`，否則當 `target = 2 * nums[i]` 時（例如 `nums = [3, 2, 4]`, `target = 6`），元素會查到自己而誤回傳 `[0, 0]`。邊走邊查、沒找到再存入，能天然保證不會重複使用同一個索引。
* **C++ STL 迭代器（Iterator）與索引（Index）區分**：
  - 需取得元素位置編號時使用整數迴圈 `for (int i = 0; i < nums.size(); i++)`；`nums.begin()` 與 `nums.end()` 回傳的是指向記憶體位置的**迭代器**，不能指派給 `int`。
  - `seen.find(key)` 回傳的是指向 `std::pair<const Key, Value>` 的迭代器（以 `auto it` 接收）；找不到時統一回傳 `seen.end()`，找到時透過 **`it->first`** 取得 Key（數值）、 **`it->second`** 取得 Value（索引）。
* **工程實務寫法習慣（C++ Code Review 標準）**：
  - 優先使用 `if (auto it = seen.find(complement); it != seen.end())` 搭配 `it->second`，而非 `if (seen.count(complement)) { return {seen[complement], i}; }`。前者僅需計算一次雜湊值且為純唯讀操作，可避免兩次雜湊查詢開銷，也能避開 `operator[]` 在鍵值不存在時意外自動插入預設節點的副作用。

---

## 5. 程式碼實作 (C++)

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // Key: 數字的值 (nums[i]), Value: 該數字的索引 (i)
        std::unordered_map<int, int> seen;

        for (int i = 0; i < nums.size(); i++) {
            int complement = target - nums[i];

            // 單次雜湊查詢（C++17 if 初始化語句），只查詢先前已走訪的元素
            if (auto it = seen.find(complement); it != seen.end()) {
                return {it->second, i};
            }

            // 尚未配對成功，將當前數值與索引存入雜湊表
            seen[nums[i]] = i;
        }

        return {};
    }
};
```