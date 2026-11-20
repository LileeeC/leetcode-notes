# [0203] 移除鏈結串列元素 (Remove Linked List Elements)

- **難度**：Easy
- **標籤**：`Linked List` `Recursion`
- **刷題日期**：2026-11-13
- **複習狀態**：🟢 一次 AC

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定一個單向鏈結串列的頭節點 `head` 與一個整數 `val`，刪除串列中所有節點值等於 `val` 的節點，並回傳新的頭節點。
* **輸入限制**：
  - 節點數量範圍為 $[0, 10^4]$（輸入可能是空串列 `head == nullptr`）。
  - $1 \le \text{Node.val} \le 50$，$0 \le \text{val} \le 50$。
* **特殊狀況 (Edge Cases)** ：
  - 空串列輸入（`head == nullptr`）。
  - 頭節點 `head` 本身就是目標值，甚至前幾個連續節點都要刪除（例如 `[7, 7, 1, 2]`, `val = 7`）。
  - 串列中所有節點都等於 `val`，刪除後變成空串列（例如 `[7, 7, 7]`, `val = 7`，應回傳 `nullptr`）。
  - 連續出現多個需刪除的目標節點（例如 `[1, 2, 2, 3]`, `val = 2`）。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🔴 直接操作原串列 (Without Dummy Head) - 初始想法
* **直覺**：若要刪除某個節點，指標必須停在它的前一個節點（`prev->next = curr->next`）。但若 `head` 本身就要被刪除，`head` 前方沒有節點，必須額外撰寫一個 `while (head != nullptr && head->val == val)` 迴圈單獨處理頭節點移動。
* **瓶頸**：將「刪除頭節點」與「刪除中間節點」拆成兩套邏輯，程式碼冗長且容易在連續刪除頭節點時發生空指標存取錯誤。

### 🟢 最佳化解法 (虛擬頭節點 Dummy Head / Sentinel Node)
* **核心突破口**：在原本的 `head` 前方接上一個虛擬頭節點 `dummy`（呼叫建構子 `ListNode dummy(0, head);`），讓原本的 `head` 變成普通的中間節點，使所有節點都能套用完全一致的刪除邏輯。
* **演算法步驟**：
  1. 在 Stack 上建立虛擬頭節點 `ListNode dummy(0, head);`（函式結束會自動回收，不需手動 `delete dummy`），並令走訪指標 `curr = &dummy`。
  2. 當 `curr->next != nullptr` 時，檢查下一個節點的值：
     - 若 `curr->next->val == val`：先暫存 `toDelete = curr->next`，將 `curr->next` 指向 `curr->next->next` 跨過目標節點，並 `delete toDelete` 釋放記憶體。**此時 `curr` 保持不動**，下一輪繼續檢查新接過來的 `curr->next`。
     - 若 `curr->next->val != val`：確認安全保留，`curr = curr->next` 向前推進一步。
  3. 迴圈結束後，回傳 `dummy.next` 即為真正的新頭節點。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**：$O(N)$ — 指標 `curr` 從虛擬頭節點出發，線性走訪整個鏈結串列一次。
* **空間複雜度**：$O(1)$ — 僅在 Stack 配置一個常數大小的 `dummy` 節點與指標變數，無額外動態記憶體開銷。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **刪除節點後 `curr` 不可移動**：當 `curr->next` 被刪除並接上 `curr->next->next` 後，新接過來的節點尚未被檢查過，因此**只有在 `else`（未發生刪除）時才能執行 `curr = curr->next`**，否則會漏刪連續重複的目標值（如 `[1, 2, 2, 3]` 刪 `2`）。
* **Stack 宣告 `dummy` 的語法細節**：`ListNode dummy(0, head);` 是在 Stack 建立實體物件並呼叫建構子，因此取得位址需用 `&dummy`，回傳新頭節點時需用點號 `dummy.next`（而非指標的 `->`）。
* **C++ 記憶體管理習慣**：被移除的串列節點是系統透過 `new` 配置在 Heap 上的，刪除連結後順手執行 `delete toDelete` 可避免記憶體洩漏（Memory Leak）。

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
class Solution1 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode* dum = new ListNode(0, head);
        ListNode* curr = dum;

        while(curr->next != nullptr){
            if(curr->next->val == val){
                ListNode* toDelete = curr->next;
                curr->next = curr->next->next;
                delete toDelete;
            }else{
                curr = curr->next;
            }
        }

        ListNode* newhead = dum->next;
        delete dum;
        return newhead;
    }
};

class Solution2 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // 直接在 Stack 建立區域變數，函式結束會自動回收，不需手動 delete dummy
        ListNode dummy(0, head);
        ListNode* curr = &dummy;

        while(curr->next != nullptr){
            if(curr->next->val == val){
                ListNode* toDelete = curr->next;
                curr->next = curr->next->next;
                delete toDelete;
            }else{
                curr = curr->next;
            }
        }
        return dummy.next;
    }
};
```