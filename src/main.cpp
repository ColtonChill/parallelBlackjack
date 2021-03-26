#include <iostream>  // cout/cin

// #include "game.hpp" // header in local directory
#include "game/card.hpp"

int main(){
    Card card = Card(0);
    
    std::cout<<card.score()<<std::endl;
    std::cout<<card.getSuit()<<std::endl;
    std::cout<<card.getRank()<<std::endl;
    std::cout<<card.str()<<std::endl;
    std::cout<<card<<std::endl;

    return 0;
}