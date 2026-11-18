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
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        ListNode* tmp = dummy;
        ListNode* curr = dummy;
        for(int i = 0; i < n; i++) tmp = tmp->next;
        while(tmp->next != nullptr){
            curr = curr->next; 
            tmp = tmp->next;
        }
        ListNode* todelete = curr->next;
        curr->next = curr->next->next;

        head = dummy->next;
        delete todelete;
        delete dummy;
        
        return head;
    }
};