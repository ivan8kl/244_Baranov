#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* n = malloc(sizeof(Node));
    n->data = data;
    n->next = NULL;
    return n;
}

void reverseList(Node** head) {
    Node* prev = NULL;
    Node* curr = *head;
    while (curr) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    *head = prev;
}

void printList(Node* head) {
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    while (head) {
        Node* next = head->next;
        free(head);
        head = next;
    }
}

int main() {
    Node* head = NULL;
    Node* tail = NULL;
    int val;
    
    while (scanf("%d", &val) == 1 && val != 0) {
        Node* n = createNode(val);
        if (!head) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    
    reverseList(&head);
    printList(head);
    freeList(head);
    
    return 0;
}