#include "matrix.c"

int main() {
    int **matrix;
    int rows, cols;

    bool error = input_matrix(&matrix, &rows, &cols);

    if (!error) {
        float det;
        print_matrix(matrix, rows, cols);
        error = det_matrix(matrix, rows, cols, &det);
        printf("%.6f", det);

        free(matrix);
    }

    if (error)
        printf("n/a");

    return 0;
}

