#include "players.hpp"

//strategies
Turn hitMe(int id, Gamestate gamestate){ // 0
    return hit;
}
Turn passMe(int id, Gamestate gamestate){ // 1
    return hold;
}
Turn manuel(int id, Gamestate gamestate){ // 2
    std::string input;
    while(true){
        std::cout<<"\thit(1)/hold(2)/x?:";
        std::cin>>input;
        if(input=="1" || input=="h"){
            return hit;
        }else if(input=="2" || input=="p"){
            return hold;
        }else if(input=="x" || input=="X"){
            return pause;
        }else{
            std::cout<<"Invalid input"<<std::endl;
        }
    }
}
Turn basic(int id, Gamestate gamestate){ // 3
    Hand hand = gamestate.first[id];
    if(hand.score()<17){
        return hit;
    }else{
        return hold;
    }
}
Turn scoreing(int id, Gamestate gamestate){ // 4
    Hand hand = gamestate.first[id];
    if(hand.score()<17){
        return hit;
    }else{
        return hold;
    }
}
Turn cardCounting(int id, Gamestate gamestate){ // 5
    Hand hand = gamestate.first[id];
    if(hand.score()<17){
        return hit;
    }else{
        return hold;
    }
}


//// Player functions ////
Gambler::Gambler(int id){
    ID = id;
    state = playing;
    ai = &basic;
    hand = Hand();
}
Gambler::Gambler(int id, int* handInfo){
    ID = id;
    state = playing;
    ai = &basic;
    hand = Hand(handInfo);
}
Gambler::Gambler(int id, Logic l){
    ID = id;
    state = playing;
    ai = l;
    hand = Hand();
}
Gambler::Gambler(int id, Logic l, int* handInfo){
    ID = id;
    state = playing;
    ai = l;
    hand = Hand(handInfo);
}

void Gambler::reset(){
    hand.clear();
}
void Dealer::showHand(bool title){
    if(title) std::clog<<"Dealer:"<<std::endl;
    auto cards = hand.getCards();
    std::clog<<"\tHIDDEN, ";
    for(int i=1;i<cards.size();i++){
        std::clog<<cards[i]<<", ";
    }
    std::clog<<"(SCORE):"<<hand.score()<<std::endl;
}

void Player::showHand(bool title){
    if(title) std::clog<<"Player-"<<ID<<":"<<std::endl;
    std::clog<<"\t";
    for (auto &&card : hand.getCards()){
        std::clog<<card<<", ";
    }
    std::clog<<"(SCORE):"<<hand.score()<<std::endl;
}



