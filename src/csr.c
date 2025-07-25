#include <stdlib.h>
#include <string.h>
#include "sparsely/csr.h"

csr_t *csr_create(
    int nrows, int ncols, int nnz,
    int *rowptr, int *colind,
    double *values
) {
    if (!rowptr || !colind || !values) return NULL;

    csr_t *csr = malloc(sizeof(csr_t));
    if (!csr) return NULL;

    csr->nrows = nrows;
    csr->ncols = ncols;
    csr->nnz = nnz;
    csr->rowptr = rowptr;
    csr->colind = colind;
    csr->values = values;

    return csr;
}


void csr_destroy(csr_t *mat) {
    if (!mat) return;

    free(mat->values);
    free(mat->colind);
    free(mat->rowptr);
    free(mat);
}

static void sort_row(int *cols, double *vals, int length)
{
    for (int i = 1; i < length; i++) {
        int ckey = cols[i];
        double vkey = vals[i];
        int j = i - 1;

        while (j >= 0 && cols[j] > ckey) {
            cols[j + 1] = cols[j];
            vals[j + 1] = vals[j];
            j--;
        }

        cols[j + 1] = ckey;
        vals[j + 1] = vkey;
    }
}

void csr_sort_indices(csr_t *A) {
    for (int i = 0; i < A->nrows; i++) {
        int start = A->rowptr[i];
        int end = A->rowptr[i + 1];
        int length = end - start;

        if (length > 1) {
            sort_row(&A->colind[start], &A->values[start], length);
        }

        // Ensure diagonal last if present
        int diag_pos = -1;
        for (int k = start; k < end; k++) {
            if (A->colind[k] == i) {
                diag_pos = k;
                break;
            }
        }
        if (diag_pos >= 0 && diag_pos != end - 1) {
            // Swap diagonal to last
            int tmp_col = A->colind[diag_pos];
            A->colind[diag_pos] = A->colind[end - 1];
            A->colind[end - 1] = tmp_col;

            double tmp_val = A->values[diag_pos];
            A->values[diag_pos] = A->values[end - 1];
            A->values[end - 1] = tmp_val;
        }
    }
}