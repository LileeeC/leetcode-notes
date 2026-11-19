# [0206] 反轉鏈結串列 (Reverse Linked List)

- **難度**：Easy
- **標籤**：`Linked List` `Recursion`
- **刷題日期**：2026-11-17
- **複習狀態**：🟡 提示後完成

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定一個單向鏈結串列的頭節點 `head`，將整條串列反轉，並回傳反轉後的新頭節點。
* **輸入限制**：
  - 節點數量範圍為 $[0, 5000]$（允許空串列輸入）。
  - $-5000 \le \text{Node.val} \le 5000$。
* **特殊狀況 (Edge Cases)**：
  - 空串列輸入（`head == nullptr`，直接回傳 `nullptr`）。
  - 僅包含單一節點的串列（例如 `[1]`，反轉後仍為 `[1]`）。
  - 一般多節點串列（例如 `1 -> 2 -> 3 -> nullptr` 反轉為 `3 -> 2 -> 1 -> nullptr`）。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🟢 思路一：雙指標迭代法 (Iterative Two Pointers) — 空間最佳解
* **核心邏輯**：維護兩個指標 `prev`（初始化為 `nullptr`）與 `curr`（初始化為 `head`），在走訪過程中逐一將 `curr->next` 調頭指向 `prev`。
* **四步固定節奏**：
  1. **存後路**：進入迴圈第一時間先用 `tmp = curr->next` 記住下一個節點，避免指標轉向後遺失後續串列。
  2. **轉方向**：執行 `curr->next = prev`，將當前節點指向前驅。
  3. **移 `prev`**：`prev = curr`。
  4. **移 `curr`**：`curr = tmp`（最後一輪會自然更新為 `nullptr` 並結束迴圈）。
* **回傳結果**：迴圈終止時 `curr` 為 `nullptr`，`prev` 剛好停在原本的最後一個節點，直接回傳 `prev` 即可。

### 🟢 思路二：遞迴法 (Recursive Unwinding) — 由後往前反轉
* **核心邏輯**：先一路遞迴探底至串列最尾端抓出新的頭節點 `newHead`，再趁著函式層層退回（Unwind）的過程中，由後往前反轉指標方向並斷開舊連結。
* **遞迴流程拆解（以 `1 -> 2 -> 3 -> nullptr` 為例）**：
  1. **尋找 `newHead`**
     ```text
     reverseList(1)
       └── 呼叫 reverseList(2)
             └── 呼叫 reverseList(3)
                   └── 觸發 Base Case (3->next == nullptr)
                       回傳節點 [3] 作為 newHead
     ```
  2. **遞迴層層退回**
     * **回到 `reverseList(2)` （此時 `head = 2`，`newHead = 3`）：**
       - 執行 `head->next->next = head;`（即 `3->next = 2`）
       - 執行 `head->next = nullptr;`（即 `2->next = nullptr`）
       - 回傳 `newHead`（節點 `3`）給上一層
     * **回到 `reverseList(1)` （此時 `head = 1`，`newHead = 3`）：**
       - 執行 `head->next->next = head;`（即 `2->next = 1`）
       - 執行 `head->next = nullptr;`（即 `1->next = nullptr`）
       - 回傳 `newHead`（節點 `3`）作為最終答案

---

## 3. 複雜度分析 (Complexity Analysis)
* **迭代法 (Iterative)**：
  - **時間複雜度**：$O(N)$ — 僅走訪整條鏈結串列一次。
  - **空間複雜度**：$O(1)$ — 只使用常數個指標變數（`prev`、`curr`、`tmp`）。
* **遞迴法 (Recursive)**：
  - **時間複雜度**：$O(N)$ — 每個節點各自觸發一次遞迴呼叫與指標反轉。
  - **空間複雜度**：$O(N)$ — 遞迴深度為 $N$，需消耗系統 Call Stack 空間。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **迭代法 `tmp` 更新時機陷阱（TLE 死循環原因）**：`tmp = curr->next` 必須放在 `while (curr != nullptr)` 迴圈內部最開頭無條件執行；若放在迴圈尾端並加上 `if (curr->next != nullptr)` 判斷，當 `curr` 走到最後一個節點時 `tmp` 會停止更新並繼續指著自己，導致下一輪 `curr = tmp` 陷入無窮迴圈（TLE）。此外，在迴圈外預先執行 `tmp = curr->next` 也會在空串列（`head == nullptr`）時觸發空指標崩潰。
* **遞迴法必須執行 `head->next = nullptr`**：若只寫 `head->next->next = head` 而忘記斷開原本的 `head->next`，當退回最外層（節點 `1`）時，節點 `1` 與節點 `2` 會互相指向對方形成環狀死循環。
* **工程與面試選擇**：迭代法空間複雜度為 $O(1)$ 且無 Stack Overflow 風險，為實務首選；遞迴法則是後續練習「反轉部分鏈結串列（LC 92）」與「K 個一組反轉（LC 25）」的重要思維基礎。

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
class Solution_iteration {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        ListNode* tmp;
        

        while(curr != nullptr){
            tmp = curr->next;
            head = curr;
            curr->next = prev;
            prev = curr;
            curr = tmp;
        }

        return head;
    }
};

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
class Solution_recursion {
public:
    ListNode* reverseList(ListNode* head) {
        if(head == nullptr || head->next == nullptr){
            return head;
        }
        // 遞迴反轉 head 後方的所有節點，並接住整條串列的新頭節點
        ListNode* newhead = reverseList(head->next);
        // 反轉當前節點與下一個節點的指標方向
        head->next->next = head;
        head->next = nullptr; // 必須置空，否則原本的第一個節點會與第二個節點形成死循環

        // 將新頭節點一路往上層回傳
        return newhead;
    }
};
```