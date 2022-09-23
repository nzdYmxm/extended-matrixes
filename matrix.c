#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#ifndef bool
#define true 1
#define false 0
#define bool int
#endif

// Matrix library by verdaqui

// Allocates matrix with given size.
// Returns true if error has occured, false otherwise.
// If error has occured, there is no need to free the memory
bool create_matrix(int ***matrix, int rows, int cols);

// Inputs matrix size via stdin.
// Returns true is error has occured, false otherwise
bool input_matrix_size(int *rows, int *cols);

// Returns true, if given scanf data means error
bool has_scan_error(int result, char tmp);

// Uses stdin to fill the matrix with given size.
// Matrix must be already allocated for this to work.
// Returns true if some error has occured
bool input_matrix_content(int **matrix, int rows, int cols);

// Prints out matrix via stdout.
void print_matrix(int **matrix, int rows, int cols);

// Inputs matrix size via stdin, allocates it, fills it via stdin.
// Returns true is error has occured. If error is occured, there is no need
// to deallocate the matrix.
bool input_matrix(int ***matrix, int *rows, int *cols);

// Clones matrix into `clone` with full data copy.
// Returns true if error has ocured. There is no need to deallocate new matrix in case of error
bool clone_matrix(int **matrix, int rows, int cols, int ***clone, int *clone_rows, int *clone_cols);

// Clones data from one matrix to another with no allocation.
void clone_into_matrix(int **from, int rows, int cols, int **to);

// Creates + allocates new matrix at `result` and sums mat_1 and mat_2 into it.
// Returns true if error has occured. In case of error there is no need to deallocate
// result matrix.
bool sum_matrices(int **mat_1, int rows_1, int cols_1,
                  int **mat_2, int rows_2, int cols_2,
                  int*** result, int *res_rows, int *res_cols);

// Replaces already existing matrix with transposed one.
// Returns true if error has occured.
bool transpose_matrix(int ***matrix, int *rows, int *cols);

// Multiplies two matrices and creates/allocates new one at `result`.
// Returns true if error has occured. In case of error there is no need to deallocate result matrix
bool mul_matrices(int **mat_1, int rows_1, int cols_1,
                  int **mat_2, int rows_2, int cols_2,
                  int*** result, int *res_rows, int *res_cols);

// Calculates and returns determinant of the given part of the matrix
bool det_matrix(int **matrix, int rows, int cols, float *result);


bool det_matrix(int **matrix, int rows, int cols, float *result) {
    bool error = rows != cols;
    if (!error) {
        if (rows == 0) {
            error = true;
        } else if (rows == 1) {
            (*result) = (float) matrix[0][0];
        } else {
            float sum = 0.0;

            for (int i = 0; i < rows; i++) {
                float local;

                int **new_matrix;
                create_matrix(&new_matrix, rows - 1, cols - 1);

                int new_i = 0;
                for (int ni = 0; ni < rows; ni++) {
                    if (ni != i) {
                        for (int nj = 1; nj < cols; nj++) {
                            new_matrix[new_i][nj - 1] = matrix[ni][nj];
                        }
                        new_i++;
                    }
                }

                error = det_matrix(new_matrix, rows - 1, cols - 1, &local);

                free(new_matrix);

                if (error) {
                    break;
                } else {
                    sum += local * powf(-1, (float)i) * matrix[i][0];
                }
            }

            (*result) = sum;
        }
    }

    return error;
}

bool mul_matrices(int **mat_1, int rows_1, int cols_1,
                  int **mat_2, int rows_2, int cols_2,
                  int*** result, int *res_rows, int *res_cols) {
    bool error = false;

    if (cols_1 != rows_2) {
        error = true;
    } else {
        (*res_rows) = rows_1;
        (*res_cols) = cols_2;
        error = create_matrix(result, *res_rows, *res_cols);

        if (!error) {
            for (int i = 0; i < rows_1; i++) {
                for (int j = 0; j < cols_2; j++) {
                    int prod_sum = 0;

                    for (int k = 0; k < cols_1; k++)
                        prod_sum += mat_1[i][k] * mat_2[k][j];

                    (*result)[i][j] = prod_sum;
                }
            }
        }
    }

    return error;
}

bool transpose_matrix(int ***matrix, int *rows, int *cols) {
    int **transposed;
    int transp_rows = *cols;
    int transp_cols = *rows;

    bool error = create_matrix(&transposed, transp_rows, transp_cols);

    if (!error) {
        for (int i = 0; i < transp_rows; i++)
            for (int j = 0; j < transp_cols; j++)
                transposed[i][j] = (*matrix)[j][i];

        free(*matrix);
        (*matrix) = transposed;
        (*rows) = transp_rows;
        (*cols) = transp_cols;
    }

    return error;
}

bool clone_matrix(int **matrix, int rows, int cols, int ***clone, int *clone_rows, int *clone_cols) {
    bool error = create_matrix(clone, rows, cols);
    (*clone_rows) = rows;
    (*clone_cols) = cols;

    if (!error) {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                (*clone)[i][j] = matrix[i][j];
    }

    return error;
}

void clone_into_matrix(int **from, int rows, int cols, int **to) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            to[i][j] = from[i][j];
}

bool sum_matrices(int **mat_1, int rows_1, int cols_1,
                  int **mat_2, int rows_2, int cols_2,
                  int*** result, int *res_rows, int *res_cols) {
    bool error = false;

    if (rows_1 != rows_2 || cols_1 != cols_2) {
        error = true;
    } else {
        (*res_rows) = rows_1;
        (*res_cols) = cols_1;
        create_matrix(result, rows_1, cols_1);

        for (int i = 0; i < rows_1; i++)
            for (int j = 0; j < cols_1; j++)
                (*result)[i][j] = mat_1[i][j] + mat_2[i][j];
    }

    return error;
}

bool input_matrix(int ***matrix, int *rows, int *cols) {
    bool is_allocated = false;

    bool error = input_matrix_size(rows, cols);

    if (!error)  {
        error = create_matrix(matrix, *rows, *cols);

        if (!error) {
            is_allocated = true;
            error = input_matrix_content(*matrix, *rows, *cols);
        }
    }

    if (is_allocated && error)
        free(*matrix);

    return error;
}

void print_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j == 0)
                printf("%d", matrix[i][j]);
            else
                printf(" %d", matrix[i][j]);
        }

        if (i < (rows - 1))
            printf("\n");
    }
}

bool input_matrix_content(int **matrix, int rows, int cols) {
    bool error = false;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char tmp;
            int scan_result = scanf("%d%c", &(matrix[i][j]), &tmp);

            if (has_scan_error(scan_result, tmp))
                error = true;
        }

        if (error)
            break;
    }

    return error;
}

bool has_scan_error(int result, char tmp) {
    return result != 2 || !(tmp == ' ' || tmp == '\n');
}

bool input_matrix_size(int *rows, int *cols) {
    bool error = false;

    char tmp;
    int scan_result = scanf("%d%c", rows, &tmp);

    if (has_scan_error(scan_result, tmp) || (*rows) <= 0) {
        error = true;
    } else {
        scan_result = scanf("%d%c", cols, &tmp);

        if (has_scan_error(scan_result, tmp) || (*cols) <= 0) {
            error = true;
        } else {
            error = false;
        }
    }

    return error;
}

bool create_matrix(int ***matrix, int rows, int cols) {
    bool error = false;

    (*matrix) = (int**) malloc(cols * rows * sizeof(int) + rows * sizeof(int*));
    int *ptr = (int*) ((*matrix) + rows);

    if ((*matrix) == 0) {
        error = true;
    } else {
        for (int i = 0; i < rows; i++)
            (*matrix)[i] = ptr + cols * i;
    }

    return error;
}
