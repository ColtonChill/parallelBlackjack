#include <iostream>  // cout/cin
#include <mpi.h> 
#include <math.h> 
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

        int work = floor((double)iterations/(double)(size-1));
        int leftover = iterations%(size-1);

        //send work to all workers
        for(int i = 1; i < size; i++){
            //send game state
            MPI_Send(data, data[0], MPI_INT, i, 1, MCW);
            //send amount of work to do
            MPI_Send(&work, 1, MPI_INT, i, 2, MCW);
        }

        //do the extra work
        for(int i = 0; i < leftover; i++){
            //mute output
            clog.setstate(ios_base::failbit);

            //workers will simulate a single round
            temp[1]=1;
            Game game = Game(temp);
            game.play(1);

            //get results
            if(game.players[0]->state != busted){
                result = 1;
            } else {
                result = 0;
            }
            won+=result;
        }

        for(int i = 1; i < size; i++){
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &status);
            won += result;
        }
        

        //calculate results
        percent = (double)won/(double)iterations;

        //print results
        cout << "The simulation did not bust on " << won << "/" << iterations << endl; 
        cout << "That means it did not bust " << percent*100 << "% of the time" << endl;

    } else {
        int won = 0;
        int length;
        int result;

        //mute output
        clog.setstate(ios_base::failbit);

        //check for message length
        MPI_Probe(0, MPI_ANY_TAG, MCW, &status);
        
        //recieve gamestate and amount of work
        MPI_Get_count(&status, MPI_INT, &length);
        int recieved[length];
        int work;
        MPI_Recv(recieved, length, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);
        MPI_Recv(&work, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);

        while(work > 0){
            //workers will simulate a single round
            recieved[1]=1;
            Game game = Game(recieved);
            game.play(1);

            //get results
            // cout << game.players[0]->state << endl;
            if(game.players[0]->state != busted){
                result = 1;
            } else {
                result = 0;
            }
            won+=result;
            work--;
        }

        //send result back to leader
        MPI_Send(&won, 1, MPI_INT, 0, 0, MCW);
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
double strategy(int rank, int size, int iterations){
    if(rank == 0){
        int result;
        int won = 0;
        double percent;

        int strategy = 0;

        //send the strategy type and how many simulations to run
        int data[2] = {strategy, iterations/(size-1)};

        //send workers data
        for(int i = 1; i < size; i++){
            MPI_Send(data, 2, MPI_INT, i, 1, MCW);
        }

        //do the extra work
        int leftover = iterations%(size-1);

        //recieve results
        for(int i = 1; i < size; i++){
            MPI_Recv(&result, 1, MPI_INT, i, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);
            won += result;
        }

        cout << won+leftover << endl;

        //calculate results
        percent = (double)won/(double)iterations;

        //print results
        cout << "The strategy won " << won << "/" << iterations << " times" << endl; 
        cout << "That means it won " << percent*100 << "% of the time" << endl;
        
        
        //recieve results
        //calculate results
    } else {
        int work[2];
        int won = 0;
        //recieve work
        MPI_Recv(work, 2, MPI_INT, 0, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);

        //run simulation specified number of times
        for(int i = 0; i < work[0]; i++){
            if(i%2 == 0){
                won++;
            }
            //create random game state
            //simulate game using only specified strategy
        }

        //send results back to leader
        MPI_Send(&won, 1, MPI_INT, 0, 1, MCW);
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


    bustOnHit(rank, size, 30);
    // strategy(rank, size, 10);


    MPI_Finalize();
    return 0;
}
