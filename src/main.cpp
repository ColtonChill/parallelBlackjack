#include <iostream>  // cout/cin

// #include "game.hpp" // header in local directory
#include "game/deck.hpp"

int main(){
    Deck deck = Deck(1);
    Hand hand = Hand();
    Card c1 = deck.draw();
    Card c2 = deck.draw();
    Card c3 = deck.draw();
    hand.add(c1);
    hand.add(c2);
    hand.add(c3); 
    for (auto &&card : hand.getCards())
    {
        std::cout<<card<<std::endl;
    }
    
    

    std::cout<<std::endl;
    return 0;
}