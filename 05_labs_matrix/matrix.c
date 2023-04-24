#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        scanf("%lf", x++);
    }
}

void print_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        printf("%.4f ", x[i]);
    }
    printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            scanf("%lf", &A[i][j]);
        }
    }
}

void print_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            printf("%.4f ", A[i][j]);
        }
        printf("\n");
    }
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    // wymiary macierzy AB [m][n]
    double sum;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            sum = 0;
            for (int k = 0; k < p; k++){
                sum += A[i][k] * B[k][j];
            }
            AB[i][j] = sum;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    double x;
    double det = 1;
    for (int i = 0; i < n; i++){

        for (int j = i + 1; j < n; j++){
            x = A[j][i]/A[i][i];
            for (int k = i; k < n; k++){
                A[j][k] = A[j][k] - A[i][k]*x;
            }
        }
        det *= A[i][i];

    }

    return det;

}

void backward_substitution_index(double A[][SIZE], double b[], const int indices[], double x[], int n) {
    double val;
    for (int i = n - 1; i >= 0; i--){
        val = b[indices[i]];
        for (int j = n - 1; j >= i + 1; j--){
            val -= A[indices[i]][j]*x[j];
        }
        x[i] = val/A[indices[i]][i];
    }
}

double abss(double x){
    if (x < 0){
        return -1*x;
    }
    else{
        return x;
    }
}
// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps) {
    int indices[n];
    double d;
    double det = 1;
    int max;
    int t;
    int changes = 0;
    // b = NULL
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }

    for (int i = 0; i < n - 1; i++){
        // find max
        max = i;

        for (int j = i + 1; j < n; j++) {
            if (abss(A[indices[max]][i]) < abss(A[indices[j]][i])) {
                max = j;
            }
        }
        if (indices[max] != indices[i]){
            changes += 1;
        }
        t = indices[i];
        indices[i] = indices[max];
        indices[max] = t;
        if (A[indices[i]][i] == 0){
            return 0;
        }

        for (int j = i + 1; j < n; j++){
            d = A[indices[j]][i]/A[indices[i]][i];

            for (int k = i; k < n; k++){
                A[indices[j]][k] = A[indices[j]][k] - A[indices[i]][k]*d;
            }

            b[indices[j]] = b[indices[j]] - b[indices[i]]*d;
        }

    }
    for (int i = 0; i < n; i++){
        det *= A[indices[i]][i];
    }
    for (int i = 0; i < changes%2; i++)
        det *= -1;

    if (det == 0){
        return NAN;
    }
    backward_substitution_index(A, b, indices, x, n);
    return det;

}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    int indices[n];
    double d;
    double det = 1;
    int max;
    int t;
    int changes = 0;
    for (int i = 0; i < n; i++) {
        indices[i] = i;
        B[i][i] = 1;
    }

    for (int i = 0; i < n - 1; i++){
        // find max
        max = i;

        for (int j = i + 1; j < n; j++) {
            if (abss(A[indices[max]][i]) < abss(A[indices[j]][i])) {
                max = j;
            }
        }
        if (indices[max] != indices[i]){
            changes += 1;
        }
        t = indices[i];
        indices[i] = indices[max];
        indices[max] = t;
        if (A[indices[i]][i] == 0){
            return 0;
        }

        for (int j = i + 1; j < n; j++){
            d = A[indices[j]][i]/A[indices[i]][i];

            for (int k = 0; k < n; k++){
                A[indices[j]][k] = A[indices[j]][k] - A[indices[i]][k]*d;
                B[indices[j]][k] = B[indices[j]][k] - B[indices[i]][k]*d;
            }
        }

    }
    for (int i = 0; i < n; i++){
        det *= A[indices[i]][i];
    }
    for (int i = 0; i < changes%2; i++)
        det *= -1;

    if (det == 0){
        return NAN;
    }

    for (int i = n - 1; i >=  0; i--){


        if (A[indices[i]][i] == 0){
            return 0;
        }

        for (int k = 0; k < n; k++){
            B[indices[i]][k] /= A[indices[i]][i];
        }
        A[indices[i]][i] /=  A[indices[i]][i];

        for (int j = i - 1; j >= 0; j--){

            d = A[indices[j]][i]/A[indices[i]][i];

            for (int k = 0; k < n; k++){
                A[indices[j]][k] = A[indices[j]][k] - A[indices[i]][k]*d;
                B[indices[j]][k] = B[indices[j]][k] - B[indices[i]][k]*d;
            }
        }

    }

    double C[SIZE][SIZE];

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C[i][j] = B[indices[i]][j];
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            B[i][j] = C[i][j];
        }
    }

    return det;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    int m, n, p;

    scanf ("%d", &to_do);

    switch (to_do) {
        case 1:
            scanf("%d %d %d", &m, &p, &n);
            read_mat(A, m, p);
            read_mat(B, p, n);
            mat_product(A, B, C, m, p, n);
            print_mat(C, m, n);
            break;
        case 2:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", gauss_simplified(A, n));
            break;
        case 3:
            scanf("%d", &n);
            read_mat(A,n, n);
            read_vector(b, n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n", det);
            if (det) print_vector(x, n);
            break;
        case 4:
            scanf("%d", &n);
            read_mat(A, n, n);
            det = matrix_inv(A, B, n, eps);
            printf("%.4f\n", det);
            if (det) print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}