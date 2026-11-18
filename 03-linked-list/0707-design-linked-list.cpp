class MyLinkedList1 {
private:
    struct ListNode{
        int val;
        ListNode* next;
        ListNode(int x): val(x), next(nullptr){}
    };

    ListNode* dummyhead;
    int size;
public:
    MyLinkedList() {
        dummyhead = new ListNode(0);
        size = 0;
    }
    
    int get(int index) {
        if(index > (size-1) || index < 0) return -1;
        ListNode* curr = dummyhead;
        for(int i = 0; i <= index; i++){
            curr = curr->next;
        }
        return curr->val;
    }

    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    
    void addAtTail(int val) {
        addAtIndex(size, val);
    }
    
    void addAtIndex(int index, int val) {
        if(index > size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* newnode = new ListNode(val);
        newnode->next = curr->next;
        curr->next = newnode;
        size++;
    }
    
    void deleteAtIndex(int index) {
        if(index >= size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* todelete = curr->next;
        curr->next = curr->next->next;
        delete todelete;
        todelete = nullptr;
        size--;
    }
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */



class MyLinkedList2_doubly {
private:
    struct ListNode{
        int val;
        ListNode* next;
        ListNode* prev;
        ListNode(int x): val(x), next(nullptr), prev(nullptr){}
        ListNode(int x, ListNode* next, ListNode* prev): val(x), next(next), prev(prev){}
    };

    ListNode* dummyhead;
    ListNode* dummytail;
    int size;
public:
    MyLinkedList() {
        dummyhead = new ListNode(0);
        dummytail = new ListNode(0, nullptr, dummyhead);
        dummyhead->next = dummytail;
        dummyhead->prev = nullptr;
        size = 0;
    }
    
    int get(int index) {
        if(index > (size-1) || index < 0) return -1;
        ListNode* curr = dummyhead;
        for(int i = 0; i <= index; i++){
            curr = curr->next;
        }
        return curr->val;
    }

    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    
    void addAtTail(int val) {
        addAtIndex(size, val);
    }
    
    void addAtIndex(int index, int val) {
        if(index > size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* tmp = curr->next;
        ListNode* newnode = new ListNode(val);
        newnode->next = curr->next;
        newnode->prev = curr;
        curr->next = newnode;
        tmp->prev = newnode;
        size++;
    }
    
    void deleteAtIndex(int index) {
        if(index >= size || index < 0) return;
        ListNode* curr = dummyhead;
        for(int i = 0; i < index; i++){
            curr = curr->next;
        }
        ListNode* todelete = curr->next;
        curr->next = curr->next->next;
        curr->next->prev = curr;
        delete todelete;
        todelete = nullptr;
        size--;
    }
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */