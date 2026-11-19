class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        vector<int> result;
        unordered_set<int> nums1set(nums1.begin(), nums1.end());

        for(int n : nums2){
            if(nums1set.count(n)){
                result.push_back(n);
                nums1set.erase(n); // 記得erase
            }
        }
        return result;
    }
};

class Solution2 {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> result;
        unordered_set<int> nums1set(nums1.begin(), nums1.end());

        for(int n : nums2){
            if(nums1set.count(n)){
                result.insert(n);
            }
        }
        return vector<int>(result.begin(), result.end()); // 強制轉換型別
    }
};