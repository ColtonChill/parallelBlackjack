#include <iostream>  // cout/cin
#include <mpi.h> 
// #include "game.hpp" // header in local directory
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

    if(rank == 0){
        Game game = Game(2);
        game.play();
    
        int* temp = game.gameInfo();
    
        std::cout<<std::endl<<std::endl;
        temp[1] = 1;
        Game rematch = Game(temp);
        rematch.play(1,true);
    
    
        // std::cout<<temp[0]<<":"<<temp[1]<<":"<<temp[2]<<":"<<temp[3]<<std::endl;
    }

    MPI_Finalize();
    return 0;
}
