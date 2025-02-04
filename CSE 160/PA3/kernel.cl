__kernel void matrixMultiply(
    __global const int *A, __global const int *B, __global int *C,
    const unsigned int numARows, const unsigned int numAColumns,
    const unsigned int numBRows, const unsigned int numBColumns,
    const unsigned int numCRows, const unsigned int numCColumns) {
  //@@ Compute C = A^T B 
  int i = get_global_id(0);
  int j = get_global_id(1);
  int sum = 0;
  for (int k = 0; k < numBRows; k++) {
    int a = A[k*numAColumns + i];
    int b = B[k*numBColumns + j];
    sum += a * b;
  }
  C[i*numCColumns + j] = sum;

}
