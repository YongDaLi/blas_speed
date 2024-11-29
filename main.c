/**
 * Yong Da Li
 * 
 * Saturday, December 31, 2022
 * seeing how fast BLAS can be
 * 
 * matrix mult codebase copied from UTAT CCSDS123 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// #include <FLAME.h>  // AMD libflame (fork of blis): https://github.com/amd/libflame?tab=readme-ov-file
#include <cblas.h>

#include "mymatrix.h"
#include "main.h"
#include "logger.h"

void sanity_check(void){
    int nrows = 2;
    int ncols = 2;

    // test matrix * matrix
    mymatrix* mat_identity = identity_matrix(nrows, ncols);
    mymatrix* mat_rand = random_matrix(nrows, ncols, 0, 10);

    mymatrix* mat_mat_mult_result = mat_mat_mult(mat_identity, mat_rand);

    printf("identity\n");
    pretty_print_mat(mat_identity);

    printf("rand\n");
    pretty_print_mat(mat_rand);

    printf("mat mat mult result\n");
    pretty_print_mat(mat_mat_mult_result);

    // test matrix * vector
    printf("\n============\n");
    myvector* vec_rand = random_vector(ncols, 0, 10);
    myvector* mat_vec_mult_result = mat_vec_mult(mat_rand, vec_rand);

    printf("mat rand\n");
    pretty_print_mat(mat_rand);

    printf("vector random\n");
    pretty_print_vec(vec_rand);

    printf("mat vec mult result\n");
    pretty_print_vec(mat_vec_mult_result);

    // clean up
    del_matrix(mat_identity);
    del_matrix(mat_rand);
    del_matrix(mat_mat_mult_result);
    del_vector(vec_rand);
    del_vector(mat_vec_mult_result);
}


/**
 * returns execution time in milliseconds
 */
int test_mymatrix_mat_mat_mult(int nrows, int ncols, int N){
    printf("starting test, nrows=%3d, ncols=%3d, N=%3d\n", nrows, ncols, N);

    mymatrix* matA = random_matrix(nrows, ncols, -1000, 1000);
    mymatrix* matB = random_matrix(nrows, ncols, -1000, 1000);

    mymatrix* result;

    // start test
    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        // print progress every 10%
        if (i % (int)(N * 0.10) == 0) {
            printf("%3d/%3d\n", i, N);
        }
        result = mat_mat_mult(matA, matB);
    }
    clock_t stop = clock();

    clock_t diff = stop - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;

    printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);

    // clean up
    del_matrix(matA);
    del_matrix(matB);
    del_matrix(result);

    return msec;
}


/**
 * returns execution time in milliseconds
 */
int test_blas_mat_mat_mult(int nrows, int ncols, int N){
    printf("starting test, nrows=%3d, ncols=%3d, N=%3d\n", nrows, ncols, N);

    // create
    double* A = malloc(sizeof(double) * nrows * ncols);
    double* B = malloc(sizeof(double) * nrows * ncols);
    double* C = malloc(sizeof(double) * nrows * ncols);

    int n = nrows;

    int min = -1000;
    int max = 1000;

    // fill with RNG
    for (int i=0; i<nrows*ncols; i++){
        A[i] = (double) (rand()%(max-min) + min);
        B[i] = (double) (rand()%(max-min) + min);
    }
    
    clock_t start = clock();
    // compute N times
    for (int i=0; i<N; i++){
        // print progress every 10%
        if (i % (int)(N * 0.10) == 0) {
            printf("%3d/%3d\n", i, N);
        }
        // lblas normal
        // cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);

        // AMD Ryzen BLIS
        // cblas_dgemm(enum CBLAS_ORDER Order, enum CBLAS_TRANSPOSE TransA,
        //                                   enum CBLAS_TRANSPOSE TransB, f77_int M, f77_int N,
        //                                   f77_int K, double alpha, const double* A,
        //                                   f77_int lda, const double* B, f77_int ldb,
        //                                   double beta, double* C, f77_int ldc);
        // cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);


        // AMD Ryzen libflame (AMD fork of blis)
        // void cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
        //          const enum CBLAS_TRANSPOSE TransB, const integer M, const integer N,
        //          const integer K, const double alpha, const double *A,
        //          const integer lda, const double *B, const integer ldb,
        //          const double beta, double *C, const integer ldc);
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);      // liyongda (29-Nov-2024): use dgemm for double precision float instead of cgemm=complex 
    }
    clock_t stop = clock();
    clock_t diff = stop - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;

    printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);

    // clean up
    free(A);
    free(B);

    return msec;
}


int main(int argc, char* argv[]){
    srand((unsigned int) time(NULL));
    sanity_check();

    int nrows = 100;    // assume square matrix
    int N = 1000;

    int msec_mymatrix = 0;
    int msec_blas = 0;

    printf("===== mymatrix (float, 32-bit single precision) ===== \n");
    msec_mymatrix = test_mymatrix_mat_mat_mult(nrows, nrows, N);

    // printf("===== AMD Ryzen blas (double, 64-bit double precision)===== \n");
    printf("===== Netlib blas (double, 64-bit double precision)===== \n");
    msec_blas = test_blas_mat_mat_mult(nrows, nrows, N);

    printf("==== conclusion ====\n");
    printf("The second one was %i times faster.\n", msec_mymatrix/msec_blas);

    return 0;
}