#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    
    int *mat = malloc(n * m * sizeof(int));
    for (int i = 0; i < n * m; i++)
        scanf("%d", &mat[i]);
    
    int *trans = malloc(m * n * sizeof(int));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            trans[j * n + i] = mat[i * m + j];
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", trans[i * n + j]);
        printf("\n");
    }
    
    free(mat);
    free(trans);
    return 0;
}