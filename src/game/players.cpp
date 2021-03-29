#include "players.hpp"

void Gambler::reset(){
    state = playing;
    hand.clear();
}
Dealer::Dealer(){
    name = "Dealer";
}
void Dealer::showHand(){
    std::cout<<"Player:"<<name<<":"<<std::endl;
    auto cards = hand.getCards();
    std::cout<<"HIDDEN, ";
    for(int i=1;i<cards.size();i++){
        std::cout<<cards[i]<<", ";
    }
    std::cout<<std::endl;
}

void Player::showHand(){
    std::cout<<"Player:"<<name<<":"<<std::endl;
    for (auto &&card : hand.getCards()){
        std::cout<<card<<", ";
    }
    std::cout<<std::endl;
}
Turn Human::takeTurn(){
    std::string input;
    while(true){
        std::cout<<"hit(1)/hold(2)?:";
        std::cin>>input;
        if(input=="1"){
            return hit;
        }else if(input=="2"){
            return hold;
        }else{
            std::cout<<"Invalid input"<<std::endl;
        }
    }
}