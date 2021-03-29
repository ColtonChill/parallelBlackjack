#include "game.hpp"


Game::Game(int* info){
    
}
Game::Game(int numPlayers){
    deck = Deck();
    dealer = Dealer();

    players.push_back(new Human("user"));
    for(int i=1;i<numPlayers;i++){
        players.push_back(new NPC(std::to_string(i)));
    }
}
void Game::reset(){
    dealer.reset();
    for (auto &&p : players) p->reset();
}


bool Game::isAnyonePlaying(){
    for (auto &&p : players){
        if(p->state==playing) return true;
    }
    return dealer.state==playing;
}
void Game::doAction(Gambler *player){
    if(player->takeTurn()==hit){
        player->hand.add(deck.draw());
        if(player->hand.score()>21) player->state=busted;
    }else{
        player->state = holding;
    }
}


void Game::play(){
    reset();
    // deal out cards
    for(int i=0;i<2;i++) dealer.hand.add(deck.draw());
    for(int p=0;p<players.size();p++){
        for(int i=0;i<2;i++) players[p]->hand.add(deck.draw());
    }
    dealer.showHand();
    // game loop
    while(isAnyonePlaying()){ // exit when all player.states != playing
        for (auto &&p : players){
            p->showHand();
            doAction(p);
        }
        dealer.showHand();
        doAction(&dealer);
    }

    // determine winners
    if(dealer.state==busted){
        for (auto &&player : players){
            if(player->state==holding){
                player->state==win;
            }else{
                player->state==lose;
            }
        }
    }else{ //dealer==holding
        for (auto &&player : players){
            if(player->hand.score()>dealer.hand.score()){
                player->state==win;
            }else{
                player->state==lose;
            }
        }
    }

}