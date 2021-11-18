#include <iostream>

struct Node {
    int index;
    char* buffer;
    Node* next;
};

struct List {
    Node* head;
    Node* tail;
};

List list;

void InitializeList() {
    for (int i = 0; i < 128; i++) {
        Node* node = (Node*)malloc(sizeof(Node));
        node->index = i + 1;
        node->buffer = (char*)malloc(1024);
        node->next = NULL;
        if (list.head == NULL) {
            list.head = node;
        }
        else {
            list.tail->next = node;
        }
        list.tail = node;
    }
}

char* GetNewBufferFromPool() {
    if (list.head == NULL)
        return NULL;

    char* retVal = list.head->buffer;

    Node* temp = list.head;
    list.head = list.head->next;
    if (list.head == NULL)
        list.tail = NULL;
    free(temp);
  
    return retVal;
}

void ReturnBufferToPoll(char* buffer) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->index = (list.tail->index) % 128 + 1;
    node->buffer = buffer;
    node->next = NULL;
    list.tail->next = node;
    list.tail = node;
}

int main()
{
    InitializeList();
    for (int i = 0; i < 130; i++) {
        char* buffer = GetNewBufferFromPool();
        ReturnBufferToPoll(buffer);
    }
}

