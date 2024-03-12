#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int rows, cols;
int n1, m1, n2, m2;
int a[20][20]; int b[20][20]; 
int c1[20][20];
int c2[20][20];
int c3[20][20];

typedef struct {
    int row;
    int col;
} thread_args;

void *multiplication_per_matrix(void *args){
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            c1[i][j] = 0;
            for (int k = 0; k < m1; k++) {
                c1[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

void *multiplication_per_line(void *args){
    thread_args *arg = (thread_args *)args;
    int i = arg->row;
    for (int j = 0; j < m2; j++) {
        c2[i][j] = 0;
        for (int k = 0; k < m1; k++) {
            c2[i][j] += a[i][k] * b[k][j];
        }
    }
    pthread_exit(NULL);
}

void *multiplication_per_element(void *args){
    thread_args *arg = (thread_args *)args;
    int i = arg->row;
    int j = arg->col;
    for (int k = 0; k < m1; k++) {
        c3[i][j] += a[i][k] * b[k][j];
    }
    pthread_exit(NULL);
}

void get_size(FILE *file) {
    rows = 0;
    cols = 0;
    fscanf(file, "row=%d col=%d", &rows, &cols);
    printf("rows %d, cols %d\n", rows, cols);
    fclose(file);
}

void read_matrix(FILE *file, int n, int m, int matrix[20][20]){
    char *line = malloc(20);
    fgets(line, 20, file);
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

void write_to_file(int matrix[20][20], int n, int m, int method) {
    char line[100] = ""; // Allocate memory for the file name
    char text[100] = ""; // Allocate memory for the text

    switch (method) {
        case 1:
            strcat(line, "c_per_matrix.txt");
            strcat(text, "Method: A thread per matrix\n");
            break;
        case 2:
            strcat(line, "c_per_line.txt");
            strcat(text, "Method: A thread per row\n");
            break;
        case 3:
            strcat(line, "c_per_element.txt"); // Add missing ".txt" extension
            strcat(text, "Method: A thread per element\n");
            break;
        default:
            break;
    }

    FILE *file = fopen(line, "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s", text);
    fprintf(file, "Rows: %d, Cols: %d\n", n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {
    // MATRIX A
    
    // Get Size of Matrix a.
    FILE *aFile;
    aFile = fopen("a.txt", "r");
    get_size(aFile);
    n1 = rows, m1 = cols;

    // Reading Matrix a.
    aFile = fopen("a.txt", "r");
    read_matrix(aFile, n1, m1, a);

    // check matrix
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < m1; j++){
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    fclose(aFile);

    // MATRIX B

    // Get Size of b.
    FILE *bFile;
    bFile = fopen("b.txt", "r");
    rows = 0, cols = 0;
    get_size(bFile);
    n2 = rows, m2 = cols;

    // Reading Matrix b.
    bFile = fopen("b.txt", "r");
    read_matrix(bFile, n2, m2, b);

    // check matrix
    for(int i = 0; i < n2; i++){
        for(int j = 0; j < m2; j++){
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    fclose(bFile);

    // MATRIX C
    // int c1[n1][m2];
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < m2; j++){
            c1[i][j] = 0;
        }
        // printf("\n");
    }

// THREADS PER MATRIX.
    clock_t start_time1 = clock();
    pthread_t thread;
    thread_args *args = malloc(sizeof(thread_args));
    pthread_create(&thread, NULL, multiplication_per_matrix, (void *)args);
    pthread_join(thread, NULL);
    clock_t end_time1 = clock();
    double total_time1 = (double)(end_time1 - start_time1) / CLOCKS_PER_SEC;
    write_to_file(c1, n1, m2, 1);


// THREADS PER ROW.
    pthread_t threads2[n1];
    thread_args args2[n1];
    clock_t start_time2 = clock();
    for(int i = 0; i < n1; i++){
        args2[i].row = i;
        pthread_create(&threads2[i], NULL, multiplication_per_line, &args2[i]);
    }
    for (int i = 0; i < n1; i++) {
        pthread_join(threads2[i], NULL);
    }
    clock_t end_time2 = clock();
    double total_time2 = (double)(end_time2 - start_time2) / CLOCKS_PER_SEC;
    write_to_file(c2, n1,m2,2);


// THREADS PER ELEMENT.
    pthread_t threads3[n1*m2];
    thread_args args3[n1*m2];
    int count = 0;
    clock_t start_time3 = clock();
    for(int i = 0; i < n1; i++){
        for(int j = 0; j < m2; j++){
            args3[count].row = i;
            args3[count].col = j;
            pthread_create(&threads3[count], NULL, multiplication_per_element, &args3[count]);
            count++;
        }
    }
    count = 0;
    for (int i = 0; i < n1; i++) {
        for(int j = 0; j < m2; j++){
            pthread_join(threads3[count], NULL);
            count++;
        }
    }
    clock_t end_time3 = clock();
    double total_time3 = (double)(end_time3 - start_time3) / CLOCKS_PER_SEC;
    write_to_file(c3, n1,m2,3);

    printf("Method: A Thread Per Matrix takes %lf msec\n", total_time1*1000);
    printf("Method: A Thread Per Row takes %lf msec\n", total_time2*1000);
    printf("Method: A Thread Per Element takes %lf msec\n", total_time3*1000);
    return 0;
}
