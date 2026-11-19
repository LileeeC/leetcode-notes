class Solution_mine {
public:
    bool isHappy(int n) {
        unordered_set<int> cal = {};
        int add = 0;
        while(n != 1){
            while(n>0){
                add += (n%10)*(n%10);
                n /= 10;
            }
            cout << "n=" << n << ", add=" << add << '\n';
            if(cal.count(add)!= 0 && add != 1){
                cout << add << '\n';
                return false;
            }else if(cal.count(add) == 0){
                cal.insert(add);
            }
            n = add;
            add = 0;
            // for (const auto &s : cal) {
            //     cout << s << " ";
            // }
        }
        return true;
    }
};

class Solution_ver2 {
public:
    bool isHappy(int n) {
        unordered_set<int> cal;
        int add = 0;

        while(n != 1 && !cal.count(n)){
            cal.insert(n);
            while(n>0){
                add += (n%10)*(n%10);
                n /= 10;
            }
            n = add;
            add = 0;
        }
        return n == 1;
    }
};

class Solution_two_pointers {
private:
    int getNext(int n) {
        int sum = 0;
        while (n > 0) {
            int digit = n % 10;
            sum += digit * digit;
            n /= 10;
        }
        return sum;
    }

public:
    bool isHappy(int n) {
        int slow = n;
        int fast = getNext(n);

        while (fast != 1 && slow != fast) {
            slow = getNext(slow);          // 慢指標走一步
            fast = getNext(getNext(fast)); // 快指標走兩步
        }
        return fast == 1;
    }
};