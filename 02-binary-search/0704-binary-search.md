# [0704] 二分搜尋法 (Binary Search)

- **難度**：Easy
- **標籤**：`Array` `Binary Search`
- **刷題日期**：2026-11-10
- **複習狀態**：🟢 一次 AC

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **輸入限制**：輸入為已排序（升冪）的整數陣列 `nums` 與目標值 `target`。
* **前提條件**：**僅適用於已排序數列**。若陣列未排序，無法依據與 `mid` 的比較結果排除半數範圍。
* **特殊狀況 (Edge Cases)** ：
  - 陣列長度為 1。
  - 目標值不在陣列中（回傳 `-1`）。
  - `left` 與 `right` 相加時可能導致整數溢位（Overflow）。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🟢 最佳化解法 (Binary Search)
* **核心邏輯**：設定 `left`、`right` 與 `mid` 指標，每一次比較 `nums[mid]` 與 `target` 的大小，都能直接篩掉一半的搜尋範圍，將時間複雜度降至 $O(\log N)$。
* **關鍵突破口 (防溢位寫法)** ：
  - 傳統寫法 `int mid = (left + right) / 2;` 在 `left + right` 超過 `INT_MAX` 時會導致**整數溢位 (Overflow)** 。
  - 更好的寫法是 `int mid = left + (right - left) / 2;`，可以在數學上確保數值不超出 `INT_MAX`。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**：$O(\log N)$ — 每次比較均淘汰一半的資料量。
* **空間複雜度**：$O(1)$ — 只使用 `left`, `right`, `mid` 三個純量變數，記憶體開銷為常數級別。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **二分搜尋前提**：題目必須確保數列「已排序」。
* **防溢位寫法**：計算中間索引時務必採用 `left + (right - left) / 2` 避免 `INT_MAX` 溢位。
* **邊界終止條件**：`while (left <= right)` 確保當搜尋範圍只剩一個元素時（`left == right`），該元素仍會被檢查。

---

## 5. 程式碼實作 (C++)

```cpp
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) return mid;
            if (nums[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }
};

int main() {
    Solution sol;
    vector<int> nums = {-1, 0, 3, 5, 9, 12};
    int target = 9;

    int ans = sol.search(nums, target);
    cout << "Index: " << ans << endl; 

    return 0;
}
```