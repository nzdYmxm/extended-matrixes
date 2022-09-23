#include<stdlib.h>
#include<stdio.h>

#include "matrix.c"

/*
    1 6 7
    2 5 8
    3 4 9
*/
void sort_vertical(int **matrix, int n, int m, int **result_matrix);

/*
    1 2 3
    6 5 4
    7 8 9
*/
void sort_horizontal(int **matrix, int n, int m, int **result_matrix);


int main() {
    int **matrix, **result;
    int rows, cols;

    bool error = input_matrix(&matrix, &rows, &cols);

    if (!error) {
        error = create_matrix(&result, rows, cols);

        if (!error) {
            sort_vertical(matrix, rows, cols, result);
            print_matrix(result, rows, cols);
            printf("\n\n");

            sort_horizontal(matrix, rows, cols, result);
            print_matrix(result, rows, cols);

            free(matrix);
            free(result);
        }
    }

    if (error)
        printf("n/a");

    return 0;
}

int *elem_by_id_vertical(int **matrix, int rows, int id) {
    int j = id / rows;
    int i = id % rows;

    if (j % 2 == 1)
        i = rows - 1 - i;

    return &matrix[i][j];
}

void sort_vertical(int **matrix, int n, int m, int **result) {
    int size = n * m;
    clone_into_matrix(matrix, n, m, result);

    for (int i = 0; i < size; i++) {
        int min = i;

        for (int j = i + 1; j < size; j++)
            if (*elem_by_id_vertical(result, n, j) <
                *elem_by_id_vertical(result, n, min))
                min = j;

        int tmp = *elem_by_id_vertical(result, n, i);
        *elem_by_id_vertical(result, n, i) = *elem_by_id_vertical(result, n, min);
        *elem_by_id_vertical(result, n, min) = tmp;
    }
}

int *elem_by_id_horizontal(int **matrix, int cols, int id) {
    int j = id % cols;
    int i = id / cols;

    if (i % 2 == 1)
        j = cols - 1 - j;

    return &matrix[i][j];
}

void sort_horizontal(int **matrix, int n, int m, int **result) {
    int size = n * m;
    clone_into_matrix(matrix, n, m, result);

    for (int i = 0; i < size; i++) {
        int min = i;

        for (int j = i + 1; j < size; j++)
            if (*elem_by_id_horizontal(result, m, j) <
                *elem_by_id_horizontal(result, m, min))
                min = j;

        int tmp = *elem_by_id_horizontal(result, m, i);
        *elem_by_id_horizontal(result, m, i) = *elem_by_id_horizontal(result, m, min);
        *elem_by_id_horizontal(result, m, min) = tmp;
    }
}
