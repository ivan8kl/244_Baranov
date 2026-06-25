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