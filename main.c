#include <stdio.h>
#include <stdlib.h>
int rows, cols;
void get_size(FILE *file){    
    cols = 0;
    rows = 0;
    int num;
    if (file == NULL) {
        printf("Error reading file.\n");
        return;
    }
    while (fscanf(file, "%d", &num) == 1) {
        cols++;
    }
    rewind(file);
    char* line = (char*)malloc(40 * sizeof(char));
    if (line == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    while(fgets(line, 40, file))rows++;
    free(line);
    if(rows != 0)cols /= rows;
    fclose(file);
}
void read_matrix(FILE *file, int n, int m, int matrix[n][m]){
    if(file == NULL){
        printf("Error reading file.\n");
        return; 
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
}


int main() {
    // MATRIX A
    
    // Get Size of Matrix a.
    FILE *aFile;
    aFile = fopen("a.txt", "r");
    get_size(aFile);
    int n1 = rows, m1 = cols;

    // Reading Matrix a.
    int a[n1][m1];
    aFile = fopen("a.txt", "r");
    read_matrix(aFile, n1, m1, a);

    // check matrix
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < m1; j++){
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    fclose(aFile);

    // MATRIX B

    // Get Size of b.
    FILE *bFile;
    bFile = fopen("b.txt", "r");
    rows = 0, cols = 0;
    get_size(bFile);
    int n2 = rows, m2 = cols;

    // Reading Matrix b.
    int b[n2][m2];
    bFile = fopen("b.txt", "r");
    read_matrix(bFile, n2, m2, b);

    // check matrix
    for(int i = 0; i < n2; i++){
        for(int j = 0; j < m2; j++){
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }
    fclose(bFile);
    return 0;
}
