/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while(fast != nullptr && fast->next != nullptr){
            slow = slow->next;
            fast = fast->next->next;
            if(slow == fast){
                ListNode* tmp1 = head;
                ListNode* tmp2 = slow;
                while(tmp1!=tmp2){
                    tmp1 = tmp1->next; //從head走到環的起點
                    tmp2 = tmp2->next; //從快慢指標的相遇點走到環的起點
                }
                return tmp1;
            }
        }
        return nullptr;
    }
};