#include <iostream>  // cout/cin
#include <mpi.h> 
#include "game/game.hpp"

using namespace std;
#define MCW MPI_COMM_WORLD

//structure of gameinfo
// throw away size for mpi send: 0
    // auto/manuel: 1
    // deck   : 2 -> 54
    // logicType: 55
    // #player: 56
        // #hand: #57 -> 57+offset
        // #hand: #% -> %+offset

// structure of results
// 1: Playing
// 2: Holding
// 3: Win
// 4: Lose

//TODO:likelihood of busting on a hit
//Need: ability to play single round
double bustOnHit(int rank, int size, int iterations){
    MPI_Status status;
    if(rank == 0){
        int result;
        int sent = 0;
        int won = 0;
        double percent;

        //create game state
        Game game = Game(3);
        int* temp = game.gameInfo();
        int* data = temp;

        //send work to all workers
        for(int i = 1; i < size; i++){
            MPI_Send(data, data[0], MPI_INT, i, 1, MCW);
            sent++;
            iterations--;
        }

        while(iterations > 0){
            //recieve results
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &status);
            won += result;

            //send more work to workers
            MPI_Send(data, data[0], MPI_INT, status.MPI_SOURCE, 1, MCW);
            sent++;

            iterations--;
        }


        //send kill message to all processes
        for(int i = 1; i < size; i++){
            //recieve final results
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);
            won += result;

            //send kill message
            data = 0;
            MPI_Send(&data, 1, MPI_INT, i, 0, MCW);
        }

        //calculate results
        percent = won/sent;

        //print results
        cout << "The simulation did not bust on " << won << "/" << sent << " that is a " << percent*100 << "% chance" << endl;

    } else {
        int length;
        int result;
        bool keepGoing = true;

        //mute output
        clog.setstate(ios_base::failbit);

        while(keepGoing){
            //recieve gamestate
            MPI_Probe(0, MPI_ANY_TAG, MCW, &status);

            //check for kill message
            if(status.MPI_TAG == 0){
                keepGoing = false;
                cout << "Process " << rank << " has been killed" << endl;
                break;
            }

            MPI_Get_count(&status, MPI_INT, &length);

            int recieved[length];
            MPI_Recv(recieved, length, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);

            //workers will simulate a single round
            recieved[1]=1;
            Game game = Game(recieved);
            game.play(1);

            //get results
            if(1){
                result = 1;
            } else {
                result = 0;
            }

            //send result back to leader
            MPI_Send(&result, 1, MPI_INT, 0, 0, MCW);
        }

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


    bustOnHit(rank, size, 10);


    MPI_Finalize();
    return 0;
}
