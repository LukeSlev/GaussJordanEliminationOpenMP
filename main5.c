#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#include "Lab3IO.h"
#include "timer.h"

int elimination(int num_count);
void swap_row(double ***A, int k, int idx, int size);

int elimination(int num_count) {
  double ** A;
  int i,j,k;
  int l, local_max, local_idx, max, idx;
  int rows, cols;
  double temp;
  double start, finished;
  double* x;
  int* index;

  if (Lab3LoadInput(&A, &rows) == 1) { printf("Error in loading\n"); return 1;}
  cols = rows + 1;
  // PrintMat(A,rows,cols);
  // printf("rows %i  \n\n",rows);
  x = CreateVec(rows);

  index = malloc(rows * sizeof(int));
  for (i = 0; i < rows; ++i)
    index[i] = i;

  GET_TIME(start);

    printf("Before Gaussian\n\n");
    for (i = 0; i < rows; ++i){
        for (j = 0; j < cols; ++j){
            printf("%f\t", A[index[i]][j]);
        }
        printf("\n");
    }

  // Gaussian
  #pragma omp parallel num_threads(num_count) default(none) shared(A,rows,x,index,cols,max,idx) private(j,i,local_max,temp,l,k,local_idx)
  {
    for (k=0;k<rows-1;k++) {
      max=0;
      idx=0;
      #pragma omp for
      for (l=k;l<rows;l++){
        if (A[index[l]][k] * A[index[l]][k] > max ) {
          local_idx = l;
          local_max = A[index[l]][k] * A[index[l]][k];
        }
      }

      if (local_max > max) {
        #pragma omp critical
        {
          if (local_max > max) {
            max = local_max;
            idx = local_idx;
          }
        }
      }
      #pragma omp barrier
    
      #pragma omp single
      {
        if (idx != k)/*swap*/{
            l = index[idx];
            index[idx] = index[k];
            index[k] = l;
        }
      }

        #pragma omp single
        {
        printf("After swap\n\n");
        for (i = 0; i < rows; ++i){
            for (j = 0; j < cols; ++j){
                printf("%f\t", A[index[i]][j]);
            }
            printf("\n");
        }
        }

      #pragma omp for
      for (i=k+1;i<rows;i++) {
          temp = A[index[i]][k] / A[index[k]][k];
          for (j=k;j<rows+1;j++){
              A[index[i]][j] = A[index[i]][j] - temp * A[index[k]][j];
          }
      }
    }
    
    // PrintMat(A,rows,cols);
    // printf("\n\n");
    #pragma omp single
    {
      printf("Before Jordan\n\n");
      for (i = 0; i < rows; ++i){
          for (j = 0; j < cols; ++j){
              printf("%f\t", A[index[i]][j]);
          }
          printf("\n");
      }
    }
    // Jordan
    for (k=rows-1; k>0;k--){
        #pragma omp for
        for (i=0;i<k;i++){
            A[index[i]][rows] = A[index[i]][rows] - A[index[i]][k] / A[index[k]][k] * A[index[k]][rows];
            A[index[i]][k] = 0;
        }
    }

    // last step
    #pragma omp for
    for (i=0; i< rows; ++i)
        x[i] = A[index[i]][rows] / A[index[i]][i];
  }

  printf("After Jordan\n\n");
  for (i = 0; i < rows; ++i){
      for (j = 0; j < cols; ++j){
          printf("%f\t", A[index[i]][j]);
      }
      printf("\n");
  }
  printf("\n\n");
  GET_TIME(finished);

  // PrintMat(A,rows,cols);
  // printf("\n\n");
  PrintVec(x,rows);

  Lab3SaveOutput(x,rows,finished-start);
  return 0;
}

// int gauss() {
//   int i;


// }

// int jordan() {

// }


/*
Main function

*/
int main(int argc, char* argv[])
{
  int num_threads;

  if (argc != 2) {
    printf("Usage: main <number_of_threads>\n");
    return(1);
  }

  num_threads = atoi(argv[1]);

  elimination(num_threads);


  return(0);
}
