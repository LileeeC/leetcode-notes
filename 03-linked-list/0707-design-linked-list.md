# [0707] 設計鏈結串列 (Design Linked List)

- **難度**：Medium
- **標籤**：`Linked List` `Design`
- **刷題日期**：2026-11-16
- **複習狀態**：🟢 一次 AC

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：自行設計並實作一個鏈結串列類別 `MyLinkedList`（可自由選擇單向或雙向結構），支援以下五種操作：
  1. `get(index)`：取得第 `index` 個節點的值（0-indexed）；若索引無效則回傳 `-1`。
  2. `addAtHead(val)`：在第一個節點前插入值為 `val` 的新節點。
  3. `addAtTail(val)`：在最後一個節點後追加值為 `val` 的新節點。
  4. `addAtIndex(index, val)`：在第 `index` 個節點前插入新節點；若 `index == size` 則接在串列尾端；若 `index > size` 則不執行任何動作。
  5. `deleteAtIndex(index)`：若索引有效，刪除第 `index` 個節點。
* **輸入限制**：
  - $0 \le \text{index}, \text{val} \le 1000$
  - 所有操作函式的總呼叫次數不超過 `2000` 次。
* **特殊狀況 (Edge Cases)** ：
  - **空串列操作**：剛初始化（`size == 0`）時呼叫 `get(0)`、`deleteAtIndex(0)` 或 `addAtTail(val)`。
  - **邊界索引差異（極易出錯）**：
    - `addAtIndex` 的有效範圍是 `0 <= index <= size`（等於 `size` 代表插在尾端）。
    - `get` 與 `deleteAtIndex` 的有效範圍是 `0 <= index < size`（即 `index >= size` 時必須直接擋掉）。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🟢 思路一：單向鏈結串列 + 單一虛擬頭節點 (`dummyhead`)
* **核心設計**：
  - 維護一個虛擬頭節點 `dummyhead` 與串列長度 `size`，讓「在頭部操作」與「在中間操作」完全統一，省去對 `head == nullptr` 的特判。
  - **方法複用**：`addAtHead(val)` 直接呼叫 `addAtIndex(0, val)`；`addAtTail(val)` 直接呼叫 `addAtIndex(size, val)`，只需專心維護 `addAtIndex` 的指標邏輯。
  - **定位規律**：尋找目標節點的前驅時，`curr` 從 `dummyhead` 出發走 `index` 步（`i < index`），迴圈結束時 `curr` 剛好停在「第 `index` 個節點的前一個位置」。

### 🟢 思路二：雙向鏈結串列 + 頭尾雙哨兵節點 (`dummyhead` & `dummytail`)
* **核心設計**：
  - 在節點結構中加入 `prev` 指標，並在初始化時建立頭尾兩個哨兵節點互相連接（`dummyhead <-> dummytail`），確保所有真實節點永遠夾在兩個哨兵之間，插入尾端時 `curr->next` 永遠指向 `dummytail` 而非 `nullptr`，免去空指標檢查。
  - **四條指標維護順序**：插入 `newnode` 時，先暫存 `tmp = curr->next`，接好 `newnode` 的前後指標（`newnode->next = tmp`、`newnode->prev = curr`），再更新左右鄰居的指標（`curr->next = newnode`、`tmp->prev = newnode`）。
  - **(進階優化空間)** ：在 `get` 與 `addAtIndex` 中可比較 `index` 與 `size / 2`，決定從 `dummyhead` 往後走或從 `dummytail` 往前走，將走訪步數減半；若直接操作 `dummyhead->next` 與 `dummytail->prev`，`addAtHead` 與 `addAtTail` 更可達到真正的 $O(1)$。

### 💡 觀念釐清：為什麼這題不能使用二分搜尋（Binary Search）？
鏈結串列的底層記憶體特性與題目需求皆不符合二分搜尋的前提條件：

| 資料結構 | 記憶體配置 | 支援隨機存取 `arr[i]`？ | 支援二分搜尋？ | 主要優勢 |
| --- | --- | --- | --- | --- |
| **陣列 (`vector`)** | 連續記憶體 | 支援，$O(1)$ | **支援**（前提：已排序） | 查詢快，但中間插入/刪除需搬移元素 $O(N)$ |
| **鏈結串列 (`ListNode`)** | 散落記憶體（靠指標串接） | **不支援**，必須循序走訪 $O(N)$ | **不支援** | 定位後，插入/刪除只需改指標 $O(1)$，不需搬移資料 |

1. **缺乏隨機存取（Random Access）**：鏈結串列節點散落在記憶體各處，無法像陣列一樣透過位址偏移公式在 $O(1)$ 瞬間跳到 `mid`，光是走到中間節點就已經耗費 $O(N)$ 時間。
2. **操作目的與資料無序**：本題是「按指定索引 `index` 存取」而非「按目標數值搜尋」，且串列中的節點值並未排序。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**：
  - `get(index)`：$O(N)$ — 需從哨兵節點出發循序走訪 `index` 步。
  - `addAtHead(val)`：$O(1)$ — 僅需走訪 `0` 步並修改常數個指標。
  - `addAtTail(val)`：單向版本與透過 `addAtIndex(size, val)` 實作時為 $O(N)$；雙向版本若直接從 `dummytail->prev` 插入可達 $O(1)$。
  - `addAtIndex(index, val)` / `deleteAtIndex(index)`：$O(N)$ — 定位目標位置需走訪 $O(N)$ 步，修改指標本身為 $O(1)$。
* **空間複雜度**：$O(N)$ — 用於儲存 $N$ 個動態配置的 `ListNode` 節點（單次操作額外輔助空間為 $O(1)$）。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **`deleteAtIndex` 的邊界必須是 `index >= size`** ：若誤寫成 `index > size`，當傳入 `index == size` 時，`curr` 會停在最後一個真實節點，`curr->next` 為 `nullptr`，接著執行 `curr->next->next` 或 `curr->next->prev` 會直接觸發空指標崩潰（Runtime Error）。
* **指標修改順序不可顛倒**：
  - 單向插入時，務必先執行 `newnode->next = curr->next;`，再執行 `curr->next = newnode;`，否則後方串列會斷鏈遺失。
  - 雙向插入時，務必先接好 `newnode` 的 `next` 與 `prev`（或先用 `tmp` 暫存 `curr->next`），再覆寫 `curr->next`。
* **C++ 記憶體釋放習慣**：刪除節點時，先用 `todelete` 暫存目標指標，調整好前後連結後執行 `delete todelete; todelete = nullptr;`，避免記憶體洩漏與懸空指標（Dangling Pointer）。

---

## 5. 程式碼實作 (C++)

```cpp
/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */

class MyLinkedList1 {
private:
    struct ListNode{
        int val;
        ListNode* next;
        ListNode(int x): val(x), next(nullptr){}
    };

    ListNode* dummyhead;
    int size;
public:
    MyLinkedList() {
        dummyhead = new ListNode(0);
        size = 0;
    }
    
    int get(int index) {
        if(index > (size-1) || index < 0) return -1;
        ListNode* curr = dummyhead;
        for(int i = 0; i <= index; i++){
            curr = curr->next;
        }
        return curr->val;
    }

    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    
    void addAtTail(int val) {
        addAtIndex(size, val);
    }
    
    void addAtIndex(int index, int val) {
        if(index > size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* newnode = new ListNode(val);
        newnode->next = curr->next;
        curr->next = newnode;
        size++;
    }
    
    void deleteAtIndex(int index) {
        if(index >= size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* todelete = curr->next;
        curr->next = curr->next->next;
        delete todelete;
        todelete = nullptr;
        size--;
    }
};

class MyLinkedList2_doubly {
private:
    struct ListNode{
        int val;
        ListNode* next;
        ListNode* prev;
        ListNode(int x): val(x), next(nullptr), prev(nullptr){}
        ListNode(int x, ListNode* next, ListNode* prev): val(x), next(next), prev(prev){}
    };

    ListNode* dummyhead;
    ListNode* dummytail;
    int size;
public:
    MyLinkedList() {
        dummyhead = new ListNode(0);
        dummytail = new ListNode(0, nullptr, dummyhead);
        dummyhead->next = dummytail;
        dummyhead->prev = nullptr;
        size = 0;
    }
    
    int get(int index) {
        if(index > (size-1) || index < 0) return -1;
        ListNode* curr = dummyhead;
        for(int i = 0; i <= index; i++){
            curr = curr->next;
        }
        return curr->val;
    }

    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    
    void addAtTail(int val) {
        addAtIndex(size, val);
    }
    
    void addAtIndex(int index, int val) {
        if(index > size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* tmp = curr->next;
        ListNode* newnode = new ListNode(val);
        newnode->next = curr->next;
        newnode->prev = curr;
        curr->next = newnode;
        tmp->prev = newnode;
        size++;
    }
    
    void deleteAtIndex(int index) {
        if(index >= size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* todelete = curr->next;
        curr->next = curr->next->next;
        curr->next->prev = curr;
        delete todelete;
        todelete = nullptr;
        size--;
    }
};
 ```