class Solution {
public:
    vector<string> commonChars(vector<string>& words) {
        vector<string> result;
        if(words.size() == 0) return result;

        int count[26] = {0};
        for(int i = 0; i < words[0].length(); i++){ //the first string
            count[words[0][i] - 'a']++;
        }
        for(int i = 1; i < words.size(); i++){
            int count2[26] = {0};

            for(int j = 0; j < words[i].length(); j++){
                count2[words[i][j] - 'a']++;
            }
            for(int j = 0; j < 26; j++){
                count[j] = min(count[j], count2[j]);
            }
        }
        for(int i = 0; i < 26; i++){
            while(count[i] > 0){ //if 2 than output 2 times
                string str(1, i + 'a'); //char -> str
                result.push_back(str);
                count[i]--;
            }
        }
        return result;
    }
};