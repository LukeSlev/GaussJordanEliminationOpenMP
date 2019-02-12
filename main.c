#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#include "Lab3IO.h"
#include "timer.h"

int elimination(int num_threads) {
  double ** A;
  int i,j,k;
  int l, max, idx;
  int rows, cols;

  if (Lab3LoadInput(&A, &rows) == 1) { printf("Error in lodaing\n"); return 1;}
  cols = rows + 1;

  for (k=0;k<rows;k++) {
    for (l=k;l<rows;l++){
      if (l == k) {
        idx = k;
        max = A[l][k];
      }

      if (A[l][k] > max ) {
        idx = l;
        max = A[l][k];
      }
    }
    if (idx != k) {
      swap_row(&A, k, idx,rows);
    }
    PrintMat(A,rows,cols);
  }


}

void swap_row(double ***A, int k, int idx, int size) {
  int i, j;
  double * temp = CreateVec(size);

  temp = *A[k];
  *A[k] = *A[idx];
  *A[idx] = temp;
  DestroyVec(temp);
}

int gauss() {
  int i;


}

int jordan() {

}


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
