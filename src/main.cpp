#include <iostream>  // cout/cin

// #include "game.hpp" // header in local directory
#include "game/game.hpp"

int main(){
    Game game = Game(2);
    game.play();

    int* temp = game.gameInfo();

    std::cout<<std::endl<<std::endl;
    temp[1] = 1;
    Game rematch = Game(temp);
    rematch.play(1,true);


    // std::cout<<temp[0]<<":"<<temp[1]<<":"<<temp[2]<<":"<<temp[3]<<std::endl;
    return 0;
}
