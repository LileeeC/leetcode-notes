# [0242] 有效的字母異位詞 (Valid Anagram)

- **難度**：Easy
- **標籤**：`Hash Table` `String` `Sorting`
- **刷題日期**：2026-11-18
- **複習狀態**：🟢 一次 AC

---

## 1. 題目理解與邊界條件 (Input / Output / Constraints)
* **題目目標**：給定兩個字串 `s` 與 `t`，判斷 `t` 是否為 `s` 的字母異位詞（Anagram，即兩字串包含的字元種類與各字元出現次數完全相同，僅排列順序不同）。若為真回傳 `true`，否則回傳 `false`。
* **輸入限制**：
  - $1 \le \text{s.length}, \text{t.length} \le 5 \times 10^4$。
  - `s` 與 `t` 僅包含小寫英文字母（`'a'` 到 `'z'`）。
  - **Follow-up 進階追問**：若輸入字串包含任意 **Unicode 字元**，演算法與資料結構該如何調整？
* **特殊狀況 (Edge Cases)** ：
  - 兩字串長度不同（`s.size() != t.size()`）：絕對不可能是字母異位詞，可於最開頭直接剪枝回傳 `false`。
  - 兩字串完全相同（例如 `s = "abc"`, `t = "abc"`）：依定義仍屬於 Anagram，回傳 `true`。
  - 單一字元（例如 `s = "a"`, `t = "a"` 回傳 `true`；`s = "a"`, `t = "b"` 回傳 `false`）。

---

## 2. 思路演進 (Intuition & Thought Process)

### 🔴 排序比對法 (Sorting) - 初始直覺
* **直覺**：將 `s` 與 `t` 各自依字典序排序後，直接比對兩字串是否完全相等（`s == t`）。
* **瓶頸**：排序的時間複雜度為 $O(N \log N)$，且會修改原始輸入字串（或需額外 $O(N)$ 空間複製）。

### 🟢 最佳化解法：固定長度陣列作為雜湊表 (Array as Hash Table)
* **核心思想**：當資料範圍**有限且連續**時，直接用固定大小的整數陣列充當雜湊表，以字元相對於 `'a'` 的 ASCII 偏移量（`ch - 'a'`）作為索引。
* **演算法步驟**：
  1. **長度剪枝**：若 `s.size() != t.size()`，直接回傳 `false`。
  2. **建立計數陣列**：宣告長度為 26 的整數陣列 `count[26]` 並全部初始化為 `0`。
  3. **一加一減抵銷統計**：
     - 走訪 `s`，將每個字元對應的計數加 1（`count[ch - 'a']++`）。
     - 走訪 `t`，將每個字元對應的計數減 1（`count[ch - 'a']--`）。
  4. **驗證歸零**：檢查 `count` 陣列中是否 26 格皆為 `0`。若有任一格不為 `0`，代表字元頻率不符，回傳 `false`；全為 `0` 則回傳 `true`。

### 🚀 Follow-up 延伸：若輸入包含 Unicode 字元怎麼辦？
* **陣列失效原因**：Unicode 收錄超過 14 萬個字元且編碼區間不連續，開固定陣列會造成嚴重的記憶體空間浪費。
* **雜湊表選型切換**：改用 **`std::unordered_map<char32_t, int>`** 進行動態頻率統計（若在 `t` 遞減計數時發現 `count[ch] < 0`，可直接提前剪枝回傳 `false`）。
* **底層編碼實務細節**：
  - **Code Point（碼位）**：Unicode 為每個字元分配的唯一整數編號（範圍 `0` ~ `0x10FFFF`），需使用 32 位元的 **`char32_t`** 才能完整容納單一字元，避免傳統 1 byte `char` 溢位。
  - **UTF-8 解碼必要性**：在 C++ 中 `std::string` 儲存的是 UTF-8 變長位元組序列（1～4 bytes，透過前導位元 `0`、`110`、`1110`、`11110` 判斷長度，後續位元組皆以 `10` 開頭實現自同步）。若直接對 `std::string` 逐 `char`（逐 byte）計數，不同多字節字元重組後的位元組可能碰撞導致誤判，因此必須先依 UTF-8 規則解碼還原成 `char32_t` 後再統計。

---

## 3. 複雜度分析 (Complexity Analysis)
* **基礎題（僅小寫英文字母，使用固定陣列）**：
  - **時間複雜度**：$O(N)$ — 走訪 `s` 與 `t` 各一次，最後掃描固定 26 格陣列為 $O(1)$，整體為線性時間。
  - **空間複雜度**：$O(1)$ — 僅使用固定長度為 26 的整數陣列，不隨輸入字串長度 $N$ 成長。
* **Follow-up（Unicode 字元，使用 `unordered_map`）**：
  - **時間複雜度**：$O(N)$ — 雜湊表單次查詢與更新平均時間為 $O(1)$。
  - **空間複雜度**：$O(K)$ — $K$ 為字串中不重複字元的種類數（最壞情況 $K = N$）。

---

## 4. 易錯點與盲點 (Key Takeaways)
* **陣列 vs. `unordered_map` 的選型準則**：
  - **範圍小且連續**（如 26 個小寫字母、128 個 ASCII 字元）：優先用**固定大小陣列**，省去雜湊函數計算、碰撞處理與動態配置記憶體的常數開銷，Cache 命中率最高。
  - **範圍極大、稀疏或未知**（如 Unicode 字元、大範圍整數）：才使用 **`unordered_map`**，只為實際出現過的鍵值配置空間。
* **C++ Range-based `for` 迴圈寫法注意**：走訪字串或容器時（如 `for (char ch : s)`），冒號左側拿到的是**元素數值本身**而非索引；若容器內裝的是大型物件（如 `string`、`pair`），應加上 `const auto&` 避免每輪發生不必要的深拷貝。

---

## 5. 程式碼實作 (C++)

```cpp
class Solution1 {
public:
    bool isAnagram(string s, string t) {
        if(s.size() != t.size()){
            return false;
        }
        int alpha[26] = {0};
        for(int i = 0; i < s.size(); i++){
            alpha[s[i]-'a']++;
            alpha[t[i]-'a']--;
        }
        for(int i = 0; i < 26; i++){
            if(alpha[i]!= 0) return false;
        }
        return true;
    }
};

class Solution2 { //假設字串含unicode，且每個元素已是解碼後的 char32_t 或 int Code Point
public:
    bool isAnagramUnicode(const std::u32string& s, const std::u32string& t) {
        if (s.size() != t.size()) return false;

        std::unordered_map<char32_t, int> count;
        for (char32_t ch : s) {
            count[ch]++;
        }
        for (char32_t ch : t) {
            count[ch]--;
            // 小優化：若某字元次數扣成負數，代表 t 出現了 s 沒有（或更多）的字元，可提前剪枝回傳 false
            if (count[ch] < 0) return false;
        }
        return true;
    }
};
```