#include <iostream>  // cout/cin

// #include "game.hpp" // header in local directory
#include "game/game.hpp"



int main(){
    /* Mute and un-mute the clog (i.e. the ui stuff)*/
    std::clog.setstate(std::ios_base::failbit);
    std::clog.clear();


    Game game = Game(2);
    int results = game.play(3);

    int* temp = game.gameInfo();

    std::cout<<std::endl<<std::endl;


    std::cout<<"RESULTS:"<<results<<std::endl;
    Game rematch = Game(temp);
    game.setAuto(true);

    results = rematch.play(1);
    std::cout<<"RESULTS:"<<results<<std::endl;
    return 0;
}
