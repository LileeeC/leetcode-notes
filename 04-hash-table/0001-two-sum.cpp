class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // Key: 數字的值 (nums[i]), Value: 該數字的索引 (i)
        std::unordered_map<int, int> seen;

        for (int i = 0; i < nums.size(); i++) {
            int complement = target - nums[i];
            
            // 先查前面看過的元素中有沒有互補數
            auto it = seen.find(complement);
            if (it != seen.end()) {
                return {it->second, i}; // it->second 就是互補數的 index
            }
            
            // 沒找到，把自己放進 map 供後面的元素配對
            seen[nums[i]] = i;
        }
        return {};
    }
};