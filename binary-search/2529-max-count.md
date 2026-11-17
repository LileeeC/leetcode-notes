# [2529] 正整數和負整數的最大計數 (Maximum Count of Positive Integer and Negative Integer)

- **難度**：Easy
- **標籤**：`Array` `Binary Search` `Counting`
- **刷題日期**：2026-11-12
- **複習狀態**：🟢 一次 AC

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定一個**非遞減排序（已排序）**的整數陣列 `nums`，分別計算正整數（`> 0`）與負整數（`< 0`）的數量，並回傳兩者的最大值。
* **關鍵規則**：**`0` 既不是正數也不是負數**，計算數量時必須排除。
* **特殊狀況 (Edge Cases)**：
  - 陣列中包含多個 `0`（例如 `[-2, -1, 0, 0, 1]`）。
  - 陣列全為負數（例如 `[-5, -3, -1]`）或全為正數（例如 `[1, 2, 3]`）。
  - 陣列全為 `0`（例如 `[0, 0, 0]`），此時正數與負數皆為 `0`，應回傳 `0`。

## 2. 思路演進 (Intuition & Thought Process)

### 🟡 雙指標線性掃描 (Two Pointers) - 初始想法
* **直覺**：設定 `lower` 從頭往右找負數邊界，`upper` 從尾往左找正數邊界。
* **瓶頸**：最壞情況下需掃描整個陣列，時間複雜度為 $O(N)$，未充分利用陣列「已排序」的條件；且 `while` 迴圈若未先檢查索引範圍（`upper >= 0` 與 `lower < nums.size()`），在全正或全負陣列中會發生**陣列越界（Heap-Buffer-Overflow）**。

### 🟢 最佳化解法 (Binary Search: Lower Bound & Upper Bound)
* **核心邏輯**：將已排序陣列視為三個連續區塊：`[ 負數區 (< 0) | 零區 (== 0) | 正數區 (> 0) ]`，利用二分搜尋法在 $O(\log N)$ 時間內直接定位 `0` 的左右邊界：
  1. **負數個數 (`neg`)**：尋找第一個 **`>= 0`** 的位置（`lowerBound`）。該索引左邊的所有元素皆嚴格小於 `0`，因 0-indexed 特性，此索引值即等於負數總數。
  2. **正數個數 (`pos`)**：尋找第一個 **`> 0`** 的位置（`upperBound`），自動跳過所有 `0`。用總長度減去此索引（`nums.size() - idx`），即為正數總數。
* **手寫二分搜尋關鍵設計**：
  - 當 `nums[mid]` 滿足條件時**不直接 `return`**，而是先用 `ans = mid` 紀錄當前候選解，並將右邊界往左推（`right = mid - 1`），迫使搜尋繼續向左尋找「最靠左邊的第一個目標」。
  - `ans` 預設值設為 `nums.size()`：當陣列中完全找不到符合條件的元素（如全負數陣列）時，迴圈結束會安全回傳陣列總長度，與 C++ STL 的 `nums.end()` 行為一致，避免回傳 `-1` 造成算式錯誤。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**：$O(\log N)$ — 分別執行兩次二分搜尋（`lowerBound` 與 `upperBound`），每次時間為 $O(\log N)$。
* **空間複雜度**：$O(1)$ — 僅使用常數個整數指標與暫存變數，無額外記憶體配置。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **`lowerBound` 與 `upperBound` 的條件差異**：
  - `lowerBound`（找第一個非負數）：`nums[mid] >= target`
  - `upperBound`（找第一個正數，排除 0）：`nums[mid] > target`
* **預設回傳值陷阱**：尋找邊界索引時，若找不到目標應回傳 `nums.size()` 而非 `-1`，才能讓 `neg` 與 `nums.size() - posIdx` 在極端測資（全正/全負/全零）下維持數學一致性。
* **STL 速寫對照**：熟悉手寫邏輯後，在競賽或面試允許使用標準庫時，可直接用 `lower_bound(nums.begin(), nums.end(), 0) - nums.begin()` 與 `nums.end() - upper_bound(nums.begin(), nums.end(), 0)` 簡化程式碼。