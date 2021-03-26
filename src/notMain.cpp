#include <iostream>  // cout/cin
// #include <fstream>  files
#include <mpi.h>   // MPI comunication
#include <vector>  // array lists
// #include <algorithm> sort
// #include <math.h> 
// #include <cmath>
// #include <chrono> time
#include <unistd.h>
#include <string>  //string


int main(int argc, char **argv){
    int rank, size, buff;
    int flag = 0;
    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    MPI_Request request;
    srand(rank+time(NULL));

    

    MPI_Finalize();
    return 0;
};