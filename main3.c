#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#include "Lab3IO.h"
#include "timer.h"

int elimination(int num_count);
void swap_row(double ***A, int k, int idx, int size);

int elimination(int num_count) {
  double ** A;
  int i,j,k;
  int l, max, idx;
  int rows, cols;
  double temp;
  double start, finished;
  double* x;
  int* index;

  if (Lab3LoadInput(&A, &rows) == 1) { printf("Error in lodaing\n"); return 1;}
  cols = rows + 1;
  // PrintMat(A,rows,cols);
  // printf("rows %i  \n\n",rows);
  x = CreateVec(rows);

  index = malloc(rows * sizeof(int));
  for (i = 0; i < rows; ++i)
    index[i] = i;

  GET_TIME(start);

  // Gaussian
  for (k=0;k<rows-1;k++) {
    max=0;
    for (l=k, idx = 0;l<rows;l++){
      if (A[index[l]][k] * A[index[l]][k] > max ) {
        idx = l;
        max = A[index[l]][k] * A[index[l]][k];
      }
    }

    if (idx != k)/*swap*/{
      l = index[idx];
      index[idx] = index[k];
      index[k] = l;
    }

    #pragma omp parallel for num_threads(num_count) default(none) shared(A,rows,k,index) private(j,i,temp)
    for (i=k+1;i<rows;i++) {
      temp = A[index[i]][k] / A[index[k]][k];
      for (j=k;j<rows+1;j++){
        A[index[i]][j] = A[index[i]][j] - temp * A[index[k]][j];
      }
    }
  }
  // PrintMat(A,rows,cols);
  // printf("\n\n");

  // Jordan
  #pragma omp parallel for num_threads(num_count) default(none) shared(A,rows,index) private(k,i)
  for (k=rows-1; k>0;k--){
    for (i=0;i<k;i++){
      #pragma omp critical
      {
         A[index[i]][rows] = A[index[i]][rows] - A[index[i]][k] / A[index[k]][k] * A[index[k]][rows];
      }
      A[index[i]][k] = 0;
    }
  }

  // last step
  #pragma omp parallel for num_threads(num_count) default(none) shared(A,x,rows,index) private(k)
  for (k=0; k< rows; ++k)
    x[k] = A[index[k]][rows] / A[index[k]][k];

  GET_TIME(finished);

  // PrintMat(A,rows,cols);
  // printf("\n\n");
  // PrintVec(x,rows);

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