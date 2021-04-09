#include <iostream>  // cout/cin
// #include <fstream>  files
#include <mpi.h>   // MPI comunication
#include <vector>  // array lists
// #include <algorithm> sort
// #include <math.h> 
// #include <cmath>
// #include <chrono> time
#include <string>  //string

#include "game/game.hpp"

#define MCW MPI_COMM_WORLD

int main(int argc, char **argv){
    int rank, size, buff;
    int flag = 0;
    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    MPI_Request request;
    srand(rank+time(NULL));

    if(rank ==0){
        //create game state
        Game game = Game(3);
        game.play();
        int* temp = game.gameInfo();
        //send game state to workers
        MPI_Send(temp, temp[0], MPI_INT, 1, 0, MCW);

        //receive results
        //calculate results

    } else {
        int length;
        MPI_Status status;

        //receive gamestate
        MPI_Probe(0, MPI_ANY_TAG, MCW, &status);
        MPI_Get_count(&status, MPI_INT, &length);

        int received[length];
        MPI_Recv(received, length, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);

        received[1]=1;
        Game game = Game(received);
        int result = game.play();

        //workers will simulate a single round
        //send result back to leader
    }

    MPI_Finalize();
    return 0;
};