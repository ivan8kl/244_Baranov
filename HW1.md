# Homework1

---

## Task1


```c
#include <stdio.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x, y;
    printf("Enter two integers (x and y): ");
    if (scanf("%d %d", &x, &y) != 2) {
        printf("Invalid input.\n");
        return 1;
    }
    swap(&x, &y);
    printf("Values after swap: x = %d, y = %d\n", x, y);
    return 0;
}
```

---

## Task2


```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    printf("Enter the number of elements (N): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input or N must be positive.\n");
        return 1;
    }
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    for (int i = 0; i < n / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
    printf("Reversed array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
    return 0;
}
```

---

## Task3


```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m;
    printf("Enter matrix dimensions (N rows and M columns): ");
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || m <= 0) {
        printf("Invalid dimensions.\n");
        return 1;
    }
    int *mat = (int *)malloc(n * m * sizeof(int));
    if (mat == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    printf("Enter %d x %d matrix elements:\n", n, m);
    for (int i = 0; i < n * m; i++) {
        scanf("%d", &mat[i]);
    }
    int *trans = (int *)malloc(m * n * sizeof(int));
    if (trans == NULL) {
        printf("Memory allocation failed!\n");
        free(mat);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            trans[j * n + i] = mat[i * m + j]; 
        }
    }
    printf("Transposed matrix (%d x %d):\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", trans[i * n + j]);
        }
        printf("\n");
    }
    free(mat);
    free(trans);
    return 0;
}
```

---

## Task4


```c
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return NULL;
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void reverseList(Node** head) {
    Node* prev = NULL;
    Node* current = *head;
    Node* next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void freeList(Node* head) {
    Node* current = head;
    Node* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}

int main() {
    Node* head = NULL;
    Node* tail = NULL;
    int val;
    printf("Enter numbers to add to the list (input 0 to stop):\n");
    while (scanf("%d", &val) == 1 && val != 0) {
        Node* newNode = createNode(val);
        if (newNode == NULL) {
            printf("Memory allocation failed!\n");
            break;
        }
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    printf("Original list: ");
    printList(head);
    reverseList(&head);
    printf("Reversed list: ");
    printList(head);
    freeList(head);
    return 0;
}
```