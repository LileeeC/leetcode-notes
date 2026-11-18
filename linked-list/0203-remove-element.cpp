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
class Solution1 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode* dum = new ListNode(0, head);
        ListNode* curr = dum;

        while(curr->next != nullptr){
            if(curr->next->val == val){
                ListNode* toDelete = curr->next;
                curr->next = curr->next->next;
                delete toDelete;
            }else{
                curr = curr->next;
            }
        }

        ListNode* newhead = dum->next;
        delete dum;
        return newhead;
    }
};

class Solution2 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // 直接在 Stack 建立區域變數，函式結束會自動回收，不需手動 delete dummy
        ListNode dummy(0, head);
        ListNode* curr = &dummy;

        while(curr->next != nullptr){
            if(curr->next->val == val){
                ListNode* toDelete = curr->next;
                curr->next = curr->next->next;
                delete toDelete;
            }else{
                curr = curr->next;
            }
        }
        return dummy.next;
    }
};