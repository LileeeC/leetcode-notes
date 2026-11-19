# [0202] 快樂數 (Happy Number)

- **難度**：Easy
- **標籤**：`Hash Table` `Math` `Two Pointers`
- **刷題日期**：2026-11-19
- **複習狀態**：🟢 Hash Set 判環法與 Floyd 快慢指標進階法皆已掌握

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定一個正整數 `n`，判斷它是否為「快樂數（Happy Number）」。快樂數的定義為：
  1. 每次將正整數替換為它「每個位數的平方和」。
  2. 重複上述過程，若最終結果變為 `1`（並停留在 `1`），則為快樂數（回傳 `true`）；若陷入不包含 `1` 的無限循環，則不是快樂數（回傳 `false`）。
* **數學性質（為何不會無限變大？）**：
  - 對於任意 $k$ 位數，其各位數平方和的最大值為 $k \times 81$（例如 3 位數最大 `999` 變為 $3 \times 81 = 243$；10 位數最大 `2,147,483,647` 的平方和也不超過 $10 \times 81 = 810$）。
  - 因此數字在經過有限次轉換後必定被壓縮至 `243` 以內；根據鴿籠原理，若未抵達 `1`，**必定會進入有限狀態的循環（Cycle）**，不可能無限發散。
* **輸入限制**：
  - $1 \le n \le 2^{31} - 1$
* **特殊狀況 (Edge Cases)**：
  - `n = 1`：初始即為 `1`，直接回傳 `true`。
  - `n = 2` 等非快樂數：會進入 `4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4` 的固定循環，必須正確偵測並終止迴圈以避免 TLE。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🟡 方法一：雜湊集合判環法 (`std::unordered_set`)
* **核心思想**：利用 `std::unordered_set<int>` 記錄每一次轉換前出現過的數字 `n`。在迴圈 `while (n != 1 && !seen.count(n))` 中，若 `n` 曾經出現在集合中，代表數值開始重複並進入無限循環，跳出迴圈後檢查 `n == 1` 即可。
* **結構優化**：將「計算各位數平方和」抽成獨立的輔助函式 `getNext(int n)`，讓主迴圈只需專注於狀態轉移與判重，避免混用暫存累加變數導致筆誤。

### 🟢 方法二（進階最佳解）：Floyd 快慢指標判環法 (Fast & Slow Pointers)
* **核心思想**：將每一次 `getNext(n)` 的狀態轉移視為一條隱形的單向鏈結串列（與 **LC 142 Linked List Cycle II** 數學本質完全相同）。既然非快樂數必定存在環（Cycle），就不需額外開 `unordered_set` 儲存歷史節點，直接派遣**慢指標（每次走 1 步）**與**快指標（每次走 2 步）**同步前進。
* **演算法步驟**：
  1. 初始化 `slow = n`，`fast = getNext(n)`。
  2. 當 `fast != 1 && slow != fast` 時持續推進：`slow = getNext(slow)`、`fast = getNext(getNext(fast))`。
  3. 迴圈結束後，若 `fast == 1` 代表成功抵達終點（回傳 `true`）；否則代表快慢指標在環內相遇（回傳 `false`）。

---

## 3. 複雜度分析 (Complexity Analysis)
## 3. 複雜度分析 (Complexity Analysis)
單次拆解數字計算平方和（`while (n > 0)` 每次除以 10）只需走訪該數字的十進位位數，時間為 $O(\log_{10} N)$；且無論初始 $N$ 有多大，經過第 1 步壓縮後數值必落入 $\le 810$ 的固定小區間內，後續抵達 `1` 或快慢指標相遇的步數皆受限於常數上限（$O(1)$ 步）。
* **方法一（`std::unordered_set` 判環法）**：
  - **時間複雜度**：$O(\log N)$ — 瓶頸僅在於第一步拆解輸入數字 $N$ 的位數，後續在常數範圍內的查詢與插入皆為 $O(1)$。
  - **空間複雜度**：$O(\log N)$ — 需使用集合儲存走訪過的狀態數值（受限於壓縮後的常數範圍上界）。
* **方法二（Floyd 快慢指標法）**：
  - **時間複雜度**：$O(\log N)$ — 與方法一同級，且無雜湊計算常數開銷。
  - **空間複雜度**：$O(1)$ — 僅使用 `slow` 與 `fast` 兩個整數變數，完全零額外容器空間開銷。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **集合插入對象切勿誤填暫存變數**：在主迴圈內記錄歷史狀態時，必須執行 `seen.insert(n)`（紀錄當前準備轉換的數值）；若誤寫成插入尚未計算、剛歸零的累加變數 `add`（`seen.insert(0)`），集合內將永遠只有 `0`，導致非快樂數無法觸發判重而發生無窮迴圈 TLE。
* **善用 Helper Function 隔離狀態**：將 `getNext(n)` 獨立封裝後，主程式不需手動維護與重置 `add = 0`，可大幅降低變數作用域混亂帶來的 Bug，且能直接複用於快慢指標的 `getNext(getNext(fast))` 寫法。
* **演算法模式遷移**：只要題目出現「狀態依固定規則轉移、可能陷入無限循環」的描述，即使沒有實體指標結構，也能直接套用**隱式鏈結串列（Implicit Linked List）＋ 快慢指標判環**將空間複雜度壓至 $O(1)$。

---

## 5. 程式碼實作 (C++)

```cpp
class Solution {
private:
    int getNext(int n) {
        int sum = 0;
        while (n > 0) {
            int digit = n % 10;
            sum += digit * digit;
            n /= 10;
        }
        return sum;
    }

public:
    // 方法二：Floyd 快慢指標法（空間 O(1) 最佳解）
    bool isHappy(int n) {
        int slow = n;
        int fast = getNext(n);

        while (fast != 1 && slow != fast) {
            slow = getNext(slow);
            fast = getNext(getNext(fast));
        }
        return fast == 1;
    }
};