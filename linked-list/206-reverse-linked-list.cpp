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