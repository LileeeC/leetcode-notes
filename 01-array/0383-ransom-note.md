# [0383] 贖金信 (Ransom Note)

- **難度**：Easy
- **標籤**：`Hash Table` `String` `Counting`
- **刷題日期**：2026-11-20
- **複習狀態**：🟢 固定陣列計數最佳解與資料結構權衡皆已掌握

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**: 給定兩個字串 `ransomNote` 與 `magazine`，判斷 `ransomNote` 能不能由 `magazine` 裡面的字元組成。如果可以，回傳 `true`；否則回傳 `false`。
* **使用規則**:
  - `magazine` 中的**每個字元只能使用一次**（即 `magazine` 中每個字母的可用次數必須大於等於 `ransomNote` 中該字母的需求次數）。
  - 兩字串皆僅由**小寫英文字母**（`'a'` ~ `'z'`）組成。
* **輸入限制**:
  - $1 \le \text{ransomNote.length}, \text{magazine.length} \le 10^5$
  - `ransomNote` 與 `magazine` 僅包含小寫英文字母。
* **特殊狀況 (Edge Cases)**:
  - **長度不足剪枝 (Early Return)**: 若 `ransomNote.length() > magazine.length()`，即使所有字母種類都吻合，字元總數也必定不夠用，可直接回傳 `false`。
  - **相同字元重複需求**: 例如 `ransomNote = "aa"`, `magazine = "ab"`，必須嚴格檢查次數扣減後是否小於 `0`（回傳 `false`）。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🟢 最佳化解法：固定長度陣列計數法 (`int mag[26]`)
* **核心思想**: 由於題目明確限制字元範圍僅為 26 個小寫英文字母（值域極小且連續），直接宣告一個長度為 `26` 的整數陣列 `int mag[26] = {0}` 作為直接定址雜湊表（Direct-address Table），以 `c - 'a'` 作為索引統計各字母庫存。
* **演算法步驟**:
  1. **前置剪枝**: 若 `ransomNote.length() > magazine.length()`，直接回傳 `false`。
  2. **統計庫存**: 走訪 `magazine` 的每個字元 `c`，執行 `mag[c - 'a']++`。
  3. **扣減驗證**: 走訪 `ransomNote` 的每個字元 `c`，執行 `--mag[c - 'a']`；一旦發現某字母庫存小於 `0`（`--mag[c - 'a'] < 0`），代表該字母數量不足，立即回傳 `false`。
  4. 若順利走訪完畢，代表所有字元皆足夠，回傳 `true`。

### 💡 延伸思考：何時該用 `int[26]` 陣列，何時改用 `std::unordered_map`？
* **字串長度 $N$ 變大時 $\rightarrow$ 依然用固定陣列 `int[26]`**：
  - 只要字元集固定為 26 個小寫字母，無論字串長到 $10^7$，`int[26]` 永遠只佔用 $104\text{ bytes}$ 的連續記憶體（極度友善 CPU Cache），且直接下標計算 `c - 'a'` 完全沒有雜湊函數運算與指標跳轉開銷，常數時間遠快於 `unordered_map`。
* **字元種類（值域）變大或未知時 $\rightarrow$ 切換為 `std::unordered_map`**：
  - 若題目擴充為**全 Unicode 字元（如中文、Emoji，超過 14 萬種字元）**，或題目改為**「用雜誌上的單字（Words）拼湊勒索信（`vector<string>`）」**，開固定陣列會浪費大量記憶體與初始化時間；此時改用 `unordered_map<char32_t, int>` 或 `unordered_map<string, int>`，空間複雜度僅取決於實際出現的**不重複種類數 $K$**（$O(K)$）。

---

## 3. 複雜度分析 (Complexity Analysis)
* **時間複雜度**: $O(M + N)$ — 其中 $M$ 為 `magazine` 的長度，$N$ 為 `ransomNote` 的長度。最壞情況下需各線性掃描兩字串一次（且當 $N \gt M$ 時透過前置剪枝僅需 $O(1)$ 時間）。
* **空間複雜度**: $O(1)$（或記為 $O(|\Sigma|)$，其中字元集大小 $|\Sigma| = 26$ 為固定常數）— 僅使用固定大小的 `int mag[26]` 陣列，不隨輸入字串長度增加而成長。

---

## 4. 易錯點與盲點總結 (Key Takeaways)
* **剪枝 (Pruning / Early Return) 習慣**: 在字串比對與包含關係題型中，先檢查 `ransomNote.length() > magazine.length()` 能在一開始就攔截掉所有長度不符的無效輸入，減少不必要的迴圈執行。
* **陣列索引偏移與歸零初始化**: 宣告區域陣列時務必寫 `int mag[26] = {0};` 確保全部元素初始化為 `0`（否則 C++ 區域變數預設為未定義垃圾值），並透過 `c - 'a'` 將 ASCII 編碼正確映射至 `0 ~ 25`。
* **善用 Range-based `for` 迴圈**: 走訪字串字元時不需使用下標 `i`，直接寫 `for (char c : magazine)` 更簡潔安全，可避免變數名稱寫錯或索引越界。

---

## 5. 程式碼實作 (C++)

```cpp
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        // 前置剪枝 (Early Return)：若勒索信長度大於雜誌，字元數量必定不夠
        if (ransomNote.length() > magazine.length()) {
            return false;
        }

        // 統計 magazine 中各小寫字母的可用次數
        int mag[26] = {0};
        for (char c : magazine) {
            mag[c - 'a']++;
        }

        // 走訪 ransomNote 逐字扣減庫存，若小於 0 代表字元不足
        for (char c : ransomNote) {
            if (--mag[c - 'a'] < 0) {
                return false;
            }
        }

        return true;
    }
};
```