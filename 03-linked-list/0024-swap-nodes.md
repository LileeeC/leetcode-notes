# [0024] 兩兩交換鏈結串列中的節點 (Swap Nodes in Pairs)

- **難度**：Medium
- **標籤**：`Linked List` `Recursion`
- **刷題日期**：2026-11-17
- **複習狀態**：🟢 一次 AC

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定一個單向鏈結串列，兩兩交換其中相鄰的節點，並回傳交換後的新頭節點。
* **關鍵限制**：不能只是單純修改節點內部的數值（`val`），而是必須實際進行**節點指標（`next`）的重新連接**。
* **輸入限制**：
  - 節點數量範圍為 $[0, 100]$（允許空串列或奇數個節點）。
  - $0 \le \text{Node.val} \le 100$。
* **特殊狀況 (Edge Cases)** ：
  - 空串列（`head == nullptr`）或僅有一個節點（`head->next == nullptr`）：無需交換，直接回傳原 `head`。
  - 奇數個節點（例如 `1 -> 2 -> 3`）：最後剩下的單一節點 `3` 保持原位不動（結果為 `2 -> 1 -> 3`）。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🟢 最佳化解法：虛擬頭節點 + 三步驟指標交換 (Dummy Head + Iterative Pointer Swapping)
* **核心策略**：
  - 建立虛擬頭節點 `dummy` 指向 `head`，並令操作指標 `curr` 永遠停在**「準備交換的兩個節點的前一個位置」**。
  - 因為修改指標的過程中會斷開原本通往節點 `1` 與節點 `3` 的連結，必須在動手改線前先用兩個暫存指標把它們記住：
    - `tmp1 = curr->next`（記住第一個節點 `1`）
    - `tmp2 = curr->next->next->next`（記住下一組的起點 `3`）

* **三步驟指標交換圖解（以 `curr -> 1 -> 2 -> 3 -> 4 -> NULL` 為例）**：
  ```text
  初始狀態 (暫存 tmp1 = [1], tmp2 = [3])：
  [curr] ---> [1 (tmp1)] ---> [2] ---> [3 (tmp2)] ---> [4] ---> NULL

  三步驟指標重連：
               ┌─────── 步驟三 (tmp1->next = tmp2) ────────┐
               │                                           ▼
               │     ┌─ 步驟二 (curr->next->next = tmp1) ─┐
               ▼     ▼                                    │
  [curr]      [1 (tmp1)]             [2]                 [3 (tmp2)] ---> [4] ---> NULL
    │                                 ▲
    └──── 步驟一 (curr->next = [2]) ──┘

  完成單組交換並推進 curr (curr = curr->next->next)：
  [dummy] ---> [2] ---> [1 (curr)] ---> [3] ---> [4] ---> NULL
  ```

  * **執行順序拆解**[cite: 1]：
  1. **步驟一**：`curr->next = tmp1->next;`（讓 `curr` 跨過 `1`，直接指向 `2`）[cite: 1]。
  2. **步驟二**：`curr->next->next = tmp1;`（此時 `curr->next` 已經是 `2`，讓 `2` 回頭指向暫存的 `1`）[cite: 1]。
  3. **步驟三**：`tmp1->next = tmp2;`（讓已經換到後面的 `1` 接上暫存的 `3`，維持後方串列不斷鏈）[cite: 1]。
  4. **推進指標**：`curr = curr->next->next;`（`curr` 向前移動兩格，停在新位置 `1` 上，準備交換下一組 `3` 和 `4`）。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**：$O(N)$ — `curr` 每次前進兩個節點，整個鏈結串列每個節點僅被造訪與重連一次。
* **空間複雜度**：$O(1)$ — 僅使用常數個指標變數（`dummy`、`curr`、`tmp1`、`tmp2`），無額外動態空間配置。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **`while` 迴圈判斷條件順序不可顛倒**：必須寫成 `curr->next != nullptr && curr->next->next != nullptr`。利用 `&&` 的短路求值特性，先確保第一個節點存在，才能安全檢查第二個節點，否則在偶數個節點走完最後一組時會觸發空指標崩潰（Null Pointer Dereference）。
* **暫存指標的必要性**：步驟一覆寫 `curr->next` 後，原本通往節點 `1` 的路就斷了；步驟二覆寫 `2->next` 後，通往節點 `3` 的路也斷了。因此 `tmp1` 與 `tmp2` 缺一不可。
* **記憶體管理細節**：動態配置 `new ListNode(0)` 作為 `dummy` 後，回傳前先用 `head = dummy->next` 取出新頭節點，並執行 `delete dummy` 避免記憶體洩漏（或直接在 Stack 宣告 `ListNode dummy(0, head)` 省去手動釋放）。

---
## 5. 程式碼實作 (C++)

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        ListNode* curr = dummy;
        ListNode* tmp1;
        ListNode* tmp2;
        if(curr == nullptr || curr->next == nullptr) return head;
        while(curr->next != nullptr && curr->next->next != nullptr){
            tmp1 = curr->next;
            tmp2 = curr->next->next->next;
            curr->next = tmp1->next;
            curr->next->next = tmp1;
            tmp1->next = tmp2;
            curr = curr->next->next;
        }
        head = dummy->next;
        delete dummy;
        return head;
    }
};
```