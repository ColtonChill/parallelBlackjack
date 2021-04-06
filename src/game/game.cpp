#include "game.hpp"
#include <vector>

Gamestate Game::makeGameState(){
    std::vector<Hand> hands;
    hands.push_back(dealer.hand);
    for (auto &&p : players){
        hands.push_back(p->hand);
    }
    return Gamestate(hands,deck);
}

Logic Game::mapLogic(){
    switch (aiType){
    case 0:
        return &hitMe;
    case 1:
        return &passMe;
    case 2:
        return &manuel;
    case 3:
        return &basic;
    case 4:
        return &scoreing;
    case 5:
        return &cardCounting;
    default:
        return &basic;
    }
}


int* Game::gameInfo(){
    // manual/automatic ==> 0/1
    // deck state object
    // player strategies.
    // number of players (we assume a dealer and one player)
    // hands of players
    static std::vector<int> info;
    info.push_back(automatic);

    // pack deck
    auto deckInfo = deck.getInfo();
    for(int i=0;i<53;++i) info.push_back(deckInfo[i]);

    // pack player meta data
    info.push_back(aiType); // ai players will use
    info.push_back(players.size());  // number of players
    // pack dealer
    info.push_back(dealer.hand.size());
    auto handInfo = dealer.hand.getInfo(); 
    for(int i=1;i<=handInfo[0];++i){  // for size of hand
        info.push_back(handInfo[i]);
    }
    for (auto &&p : players){
        info.push_back(p->hand.size());  // size of hand
        auto handInfo = p->hand.getInfo(); 
        for(int i=1;i<=handInfo[0];++i){  // for size of hand
            info.push_back(handInfo[i]);
        }
    }

    info.insert(info.begin(),info.size()+1);  // size + the index of "size" itself.    
    return info.data(); // cast to int*
}


Game::Game(int* gamestate){ // NOTE TO SELF: need to include int flag for NPC logic unints
    // throw away size for mpi send: 0
    // auto/manuel: 1
    // deck   : 2 -> 54
    // logicType: 55
    // #player: 56
        // #hand: #57 -> 57+offset
        // #hand: #% -> %+offset

    int index = 2;
    automatic = gamestate[1];
    // Deck
    int deckInfo[53];
    for(int i=0;i<53;i++){  // parse off deck info
        deckInfo[i]=gamestate[i+index];
    }
    deck = Deck(deckInfo); index += 53;
    auto deckI = deck.getInfo();
    
    
    // player meta data
    aiType = gamestate[index];
    int playerNum = gamestate[index +1];
    // Dealer
    int dealerHandSize = gamestate[index +2];  // plus in "size" index
    int dealerHandInfo[dealerHandSize+1]; 

    index += 2;
    for(int i=0;i<=dealerHandSize;i++){
        dealerHandInfo[i] = gamestate[i+index];
    }index+=dealerHandSize+1;
    dealer = Dealer(dealerHandInfo);

    // Player
    for(int i=0;i<playerNum;i++){
        int handSize = gamestate[index];
        int handInfo[handSize];
        for(int j=0;j<=handSize;j++){
            handInfo[j] = gamestate[j+index];
        }index += handSize+1;
        if(automatic==0 && i==0){
            players.push_back(new Player(i+1,&manuel,handInfo));
        }else{
            players.push_back(new Player(i+1,mapLogic(),handInfo));
        }
    }
}

Game::Game(int numPlayers){
    deck = Deck();
    dealer = Dealer();

    for(int i=0;i<numPlayers;i++){
        if(automatic==0 && i==0){
            players.push_back(new Player(i+1,&manuel));
        }else{
            players.push_back(new Player(i+1,mapLogic()));
        }
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

bool Game::doAction(Gambler *player){
    if(player->state != playing) return false;
    Turn action = player->takeTurn(makeGameState());
    if(action==hit){
        std::clog<<"\t\thit"<<std::endl;
        player->hand.add(deck.draw());
        player->showHand(0);
        if(player->hand.score()>21){
            player->state=busted;
            std::clog<<"\t\tBUSTED"<<std::endl;
        }
    }else if(action == pause){
        return true;
    }else{
        std::clog<<"\t\thold"<<std::endl;
        player->state = holding;
    }
    return false;
}


void Game::play(int rounds, bool continuing){
    for(int r = 1;r<=rounds;r++){
        if(!continuing){
            reset();
            continuing = false;
            // deal out cards
            for(int i=0;i<2;i++) dealer.hand.add(deck.draw());
            for(int p=0;p<players.size();p++){
                for(int i=0;i<2;i++) players[p]->hand.add(deck.draw());
            }
        }
        //show everyones hands
        std::clog<<"Starting Hands:"<<std::endl;
        dealer.showHand(1);
        for (auto &&p : players){
            p->showHand(1);
        }
        std::clog<<"\nBegin Game:"<<std::endl;
        
        // game loop
        while(isAnyonePlaying()){ // exit when all player.states != playing
            std::clog<<"Turn:"<<std::endl;
            for (auto &&p : players){
                p->showHand(1);
                if(doAction(p)) return;  // for pausing the game
            }
            dealer.showHand(1);
            doAction(&dealer);
            std::clog<<std::endl;
        }

        // determine winners
        if(dealer.state==busted){
            for (auto &&player : players){
                if(player->state==holding){
                    player->state=win;
                }else{
                    player->state=lose;
                }
            }
        }else{ //dealer==holding
            for (auto &&player : players){
                if(player->hand.score()>dealer.hand.score()){
                    player->state=win;
                }else{
                    player->state=lose;
                }
            }
        }

        std::clog<<"Round "<<r<<" Results:"<<std::endl;
        std::clog<<"\tDealer:"<<dealer.state<<std::endl;
        for (auto &&p : players){
            std::clog<<"\tPlayer:"<<p->ID<<":"<<p->state<<std::endl;
        }
    }
}