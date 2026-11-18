```cpp
class Solution {
private:
    // 手寫 lower_bound：尋找第一個 >= target (0) 的索引
    int lowerBound(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        int ans = nums.size(); // 若全小於 0，回傳總長度

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] >= target) {
                ans = mid;       // 紀錄候選位置
                right = mid - 1; // 繼續向左尋找更早出現的 >= 0
            } else {
                left = mid + 1;
            }
        }
        return ans;
    }

    // 手寫 upper_bound：尋找第一個 > target (0) 的索引
    int upperBound(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        int ans = nums.size(); // 若無正數，回傳總長度

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] > target) { // 嚴格大於，跳過 0
                ans = mid;       // 紀錄候選位置
                right = mid - 1; // 繼續向左尋找第一個正數
            } else {
                left = mid + 1;
            }
        }
        return ans;
    }

public:
    int maximumCount(vector<int>& nums) {
        int neg = lowerBound(nums, 0);
        int pos = nums.size() - upperBound(nums, 0);
        
        return max(neg, pos);
    }
};  