#include <iostream>  // cout/cin
#include <mpi.h> 
// #include "game.hpp" // header in local directory
#include "game/game.hpp"

#define MCW MPI_COMM_WORLD

//TODO:likelihood of busting on a hit
//Need: ability to play single round
double bustOnHit(int rank){
    if(rank ==0){
        //create game state
        //send game state to workers
        //recieve results
        //calculate results
    } else {
        //workers will simulate a single round
        //send result back to leader
    }
                
    return 0;
}

//TODO likelihood of winning with given hand
//Need: ability to generate a specified hand
double winWithHand(int rank){
    if(rank == 0){
        //send hand to worker
        //recieve results
        //calculate results
    } else {
        //generate new game state with specified hand
        //worker simulates entire game until win/loss
        //send result back to leader
    }
    
    return 0;
}

//TODO:Success of given strategy
//Need:
double strategy(int rank){
    if(rank == 0){
        //send strategy to worker
        //recieve results
        //calculate results
    } else {
        //create random game state
        //simulate game using only specified strategy
        //send results back to leader
    }
    
    return 0;
}

int main(int argc, char **argv){
    int rank, size, buff;
    int flag = 0;
    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    MPI_Request request;
    srand(rank+time(NULL));

    if(rank == 0){
             
    }

    MPI_Finalize();
    return 0;
}
