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