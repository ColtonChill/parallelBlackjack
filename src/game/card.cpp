// #pragma once
#include "card.hpp"

Card::Card(int id){
    ID = id;
}
int Card::getID(){
    return ID;
}
int Card::score(){
    return ID%13+1;
}

std::string Card::getRank(){
    switch (ID%13+1){
    case 13:
        return "King";
    case 12:
        return "Queen";
    case 11:
        return "Jack";
    case 1:
        return "Ace";    
    default:
        return std::to_string(ID%13+1);
    }
}

std::string Card::getSuit(){
    switch (((int)ID/13)%4){
    case 0:
        return "Hearts";
    case 1:
        return "Diamonds";
    case 2:
        return "Spades";
    case 3:
        return "Clubs";
    default:
        return "ERROR:"+std::to_string(ID);
    }
}
std::string Card::str(){
    return getRank() + " of " + getSuit();
}
std::ostream &operator<<(std::ostream &os, Card &card){
    os << card.str();
    return os;
}