#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return *(A+row*cols + col);
}

void set(int cols, int row, int col, int *A, int value) {
    *(A + row*cols + col) = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            int sum = 0;
            for (int k = 0; k < colsA; k++) {
                sum += get(colsA, i, k, A) * get(colsB, k, j, B);
            }
            set(colsB, i, j, AB, sum);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            int value;
            scanf("%d", &value);
            set(cols, i, j, t, value);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for (int i=0;i<cols;i++){
        for(int j=0;j<rows;j++){
            int res = get(cols,i,j,t);
            printf("%d ", res);
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    int n = 0;
    char buff[BUF_SIZE];
    int nums[BUF_SIZE]; // maybe malloc?
    int start;
    while (fgets(buff, BUF_SIZE, stdin)){
        char *line = malloc((strlen(buff) + 1) * sizeof( char ));
        strcpy(line, buff);
        array[n] = line;
        array[n][strlen(buff)] = '\0';
        n++;
    }
    return n;
}

void write_char_line(char *array[], int n) {
    printf("%s ", array[n]);
}

void delete_lines(char *array[]) {
    while (array){
        free(array);
        array++;
    }
}

int read_int_lines_cont(int *ptr_array[]) {
    int n = 0;
    char buff[BUF_SIZE];
    int nums[BUF_SIZE]; // maybe malloc?
    int i = 0;
    int start;
    while (fgets(buff, BUF_SIZE, stdin)){
        start = i;
        for (char* ptr = strtok(buff, " "); ptr; ptr = strtok(NULL, " ")){
            nums[i] = *ptr - '0';
            i++;
        }
        nums[i] = '\0';
        i++;
        ptr_array[n] = &nums[start];
        n++;
    }
    return n;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int i = 0;
    while(ptr_array[n][i] != '\0'){
        printf("%d ",ptr_array[n][i]);
        i++;
    }
}

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int n = 0;
    char buff[BUF_SIZE];
    int *nums;
    int i;
    int sum;
    while (fgets(buff, BUF_SIZE, stdin)){
        i = 0;
        sum = 0;
        nums = (int *) malloc(sizeof (int));
        for (char* ptr = strtok(buff, " "); ptr; ptr = strtok(NULL, " ")){
            nums[i] = *ptr - '0';
            sum += *ptr - '0';
            i++;
            nums = realloc(nums, (i + 1)*sizeof (int ));
        }
        nums[i] = '\0';
        lines_array[n].values = nums;
        lines_array[n].average = sum * i;
        lines_array[n].len = i;
        n++;
    }
    return n;
}

void write_int_line(line_type lines_array[], int n) {
    for (int i = 0; i < lines_array[n].len; i++){
        printf("%d ", lines_array->values[i]);
    }
}

void delete_int_lines(line_type array[], int line_count) {
    for (int i = 0; i < line_count; i++){
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type a1 = *(line_type *)a;
    line_type b1 = *(line_type *)b;

    return a1.average - b1.average;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof (line_type), cmp);
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for (int i = 0; i < n_triplets; i++){
        scanf("%d %d %d", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet a = *(triplet *)t1;
    triplet b = *(triplet *)t2;
    if (a.r > b.r){
        return 1;
    }
    else if (a.r == b.r){
        if (a.c > b.c){
            return 1;
        }
        else{
            return -1;
        }
    }
    else{
        return -1;
    }

}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
    for (int i = 0; i < n_triplets; i++){
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
    }
    R[0] = 0;
    R[rows] = n_triplets;
    int curr_row = 0;
    int count = 0;
    for (int i = 0; i < n_triplets; i++){
        if (triplet_array[i].r == curr_row){
            count++;
        }
        else{
            R[curr_row + 1] = count;
            curr_row = triplet_array[i].r;
            count = 1;
        }
    }

}
// alternative
//for (int i = 0; i < n_triplets; i++) {
//V[i] = triplet_array[i].v;
//C[i] = triplet_array[i].c;
//}
//
//int current_row = 0;
//R[0] = 0;
//
//for (int i = 1; i <= rows; i++) {
//int count = 0;
//
//while (current_row < n_triplets && triplet_array[current_row].r == i - 1) {
//current_row++;
//count++;
//}
//
//R[i] = R[i - 1] + count;

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i < rows; i++){
        y[i] = 0;
        for (int j = R[i]; j < R[i + 1]; j++){
            y[i] += x[C[j]] * V[j];
        }
    }
}

//void read_vector(int *v, int n) { // i don't know why it doesn't work
//    for (int i = 0; i < n; i++){
//        scanf("%d ", &v[n]);
//    }
//}
void write_vector(int *v, int n) {
    for (int i = 0; i < n; i++){
        printf("%d ", v[i]);
    }

    printf("\n");
}
void read_vector(int *v, int n) {
    char buff[BUF_SIZE];
    int i = 0;
    for (char* ptr = strtok(buff, " "); ptr; ptr = strtok(NULL, " ")){
        v[i] = *ptr - '0';
        i++;
    }
    buff[0] = '\0';
    write_vector(v,n);
}



int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

