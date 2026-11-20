# [0349] 兩個陣列的交集 (Intersection of Two Arrays)

- **難度**：Easy
- **標籤**：`Array` `Hash Table` `Two Pointers` `Binary Search` `Sorting`
- **刷題日期**：2026-11-19
- **複習狀態**：🟢 雙集合法與單集合 `.erase()` 優化法皆 AC

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定兩個整數陣列 `nums1` 與 `nums2`，回傳它們的交集陣列。結果中的每個元素必須是**唯一的（Unique）**，且回傳順序不限。
* **與類似題型（LC 1002）的規格差異**：
  - **LC 1002（Find Common Characters）**：屬於多重集合（Multiset）交集，**保留重複次數**，需用陣列統計各字元出現頻率並取最小值。
  - **本題（LC 349）**：屬於嚴格集合（Set）交集，**結果強制去重**，只需紀錄元素「是否出現過」，不需統計出現次數。
* **輸入限制**：
  - $1 \le \text{nums1.length}, \text{nums2.length} \le 1000$
  - $0 \le \text{nums1}[i], \text{nums2}[i] \le 1000$
* **特殊狀況 (Edge Cases)** ：
  - 兩陣列完全沒有重疊元素：回傳空陣列 `{}`。
  - 兩陣列包含大量重複元素（例如 `nums1 = [2, 2, 2]`, `nums2 = [2, 2]`）：交集結果僅能輸出單一元素 `[2]`。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🔴 暴力雙層迴圈 / 直接以 `vector` 儲存清單
* **直覺**：將 `nums1` 存入一般 `vector`，走訪 `nums2` 時逐一線性搜尋比對，並手動檢查結果陣列是否已包含該元素。
* **瓶頸**：`vector` 沒有雜湊索引，單次查詢為 $O(N)$，整體時間複雜度退化為 $O(N \times M)$，且需額外撰寫去重判斷。

### 🟢 最佳化解法：利用 `std::unordered_set` 實現 $O(1)$ 查找與自動去重
* **核心思想**：當資料值域較大或稀疏（如一般整數）且僅需判斷「存在性」與「去重」時，使用雜湊集合 **`std::unordered_set<int>`** 是最通用的標準解。其區間建構子會自動忽略重複元素，單次查找與刪除平均時間皆為 $O(1)$。
* **兩種實作策略**：
  1. **方法一（雙集合法）**：
     - 透過區間建構子 `unordered_set<int> nums1_set(nums1.begin(), nums1.end())` 將 `nums1` 轉為無重複集合。
     - 走訪 `nums2`，若 `nums1_set.count(num)` 為真，將 `num` 插入第二個集合 `result_set`（利用集合特性自動過濾 `nums2` 中的重複命中）。
     - 最後將 `result_set` 轉回 `vector<int>` 回傳。
  2. **方法二（單集合 + `.erase()` 原地消去法，空間更優）**：
     - 同樣先建立 `nums1_set`，但改用 `vector<int> result` 直接接收答案。
     - 走訪 `nums2` 時，一旦 `nums1_set.count(num)` 命中，立刻將 `num` 推入 `result` 並執行 **`nums1_set.erase(num)`** 將該數字從集合中移除。如此後續 `nums2` 再遇到相同數字時便不會重複觸發，省去第二個 `unordered_set` 的空間與最後容器轉換開銷。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**：$O(N + M)$ — 其中 $N$ 為 `nums1` 長度，$M$ 為 `nums2` 長度。建立 `nums1_set` 需 $O(N)$，走訪 `nums2` 並進行 $O(1)$ 雜湊查詢與刪除需 $O(M)$。
* **空間複雜度**：$O(N)$ — `nums1_set` 最多儲存 $N$ 個不重複元素（若採用方法二，不需額外的 `result_set` 輔助空間；若進一步優化，可先比較兩陣列長度並將較短的陣列轉為 `unordered_set`，使輔助空間降至 $O(\min(N, M))$）。

---

## 4. 易錯點與盲點 (Key Takeaways)
* **為何不用 `vector<int>` 直接當查表清單**：將 `vector` 當作一般清單（`push_back` 存值）查詢需 $O(N)$ 且不會自動去重；除非將陣列索引視為 Key（即 Direct-Address Table，例如本題因數值範圍在 $0 \sim 1000$ 亦可開 `int hash[1001] = {0}`），否則大範圍整數一律優先使用 `unordered_set` 或 `unordered_map`。
* **`std::unordered_set` 的自動去重機制**：不論是初始化 `unordered_set<int> s(v.begin(), v.end())` 還是執行 `s.insert(x)`，底層在插入前都會先計算雜湊值檢查該鍵值是否已存在，若已存在則直接忽略，天然保證集合內元素唯一。
* **`.erase()` 技巧的適用場景**：當集合內的比對資料在「匹配成功一次後就不再需要」時，用 `set.erase(key)` 取代額外開一個 `result_set` 去重，是相當俐落的 C++ 空間優化慣用法。

---

## 5. 程式碼實作 (C++)

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        std::unordered_set<int> nums1_set(nums1.begin(), nums1.end());
        std::vector<int> result;

        for (int num : nums2) {
            if (nums1_set.count(num)) {
                result.push_back(num);
                nums1_set.erase(num); // 匹配後立即移除，避免 nums2 重複元素再次加入
            }
        }
        return result;
    }
};
```