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
        std::clog<<"\thit(1)/hold(2)/x?:";
        std::cin>>input;
        if(input=="1" || input=="h"){
            return hit;
        }else if(input=="2" || input=="p"){
            return hold;
        }else if(input=="x" || input=="X"){
            return pause;
        }else{
            std::clog<<"Invalid input"<<std::endl;
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

Turn cardCounting(int id, Gamestate gamestate){ // 4
    int myScore = gamestate.first[id].score();
    auto d = gamestate.second.getInfo();

    double odds[10];
    for(int i=0;i<9;i++){
        odds[i] = ((double)4*d[0] -d[i+1]-d[i+14]-d[i+27]-d[i+40])/(52*d[0]);
        // std::cout<<4*d[0]<<d[i+1]<<d[i+14]<<d[i+27]<<d[i+40]<<4*d[0];
        // std::cout<<" odds["<<i<<"]"<<odds[i]<<std::endl;
    }

    odds[10]=0;  // 10, jack, queen, king
    for(int i=9;i<14;i++){
       odds[10] += ((double)4*d[0] -d[i+1]-d[i+14]-d[i+27]-d[i+40])/(52*d[0]);
    }


    double chance = 0;  // chance that I won't bust on a hit
    for(int i=0;i<21-myScore;i++){
        // std::cout<<odds[i]<<" ";
        chance += odds[i];
    }
    // std::cout<<std::endl<<"chance:"<<myScore<<" "<<chance<<std::endl<<std::endl;

    if(chance>0.5){
        return hit;
    }
    for (auto &&h : gamestate.first){
        if(h.score()>myScore){
            return hit;
        }
    }
    return hold;
}

Turn psychic(int id, Gamestate gamestate){ // 5
    Hand theoreticalHand = gamestate.first[id];
    theoreticalHand.add(gamestate.second.getCards().back());
    if(theoreticalHand.score() == -1){
        return hold;
    }else{
        return hit;
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



