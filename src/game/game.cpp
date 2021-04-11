#include "game.hpp"
#include <vector>

const char* coutState[5] = {"playing", "busted", "holding", "win", "lose"};



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
        return &cardCounting;
    case 5:
        return &psychic;
    default:
        return &basic;
    }
}

int* Game::gameInfo(){
    // throw away size for mpi send: 0
    // auto/manuel: 1
    // deck   : 2 -> 54
    // logicType: 55
    // #player: 56
        // #hand: #57 -> 57+offset
        // #hand: #% -> %+offset

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
        if(i==0){
            if(automatic == 0) players.push_back(new Player(i+1,&manuel,handInfo));
            else players.push_back(new Player(i+1,mapLogic(),handInfo));
        }else{
            players.push_back(new Player(i+1,basic,handInfo));
        }
    }
}

void Game::constructor(int numPlayers, bool a){
    deck = Deck();
    dealer = Dealer();
    automatic = a;
    for(int i=0;i<numPlayers;i++){
        if(automatic==0){
            if(i==0) players.push_back(new Player(i+1,&manuel));
            else players.push_back(new Player(i+1,mapLogic()));
        }else{
            players.push_back(new Player(i+1,basic));
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
        std::clog<<"\tHIT"<<std::endl;
        player->hand.add(deck.draw());
        player->showHand(0);
        if(player->hand.score()==-1){
            player->state=busted;
            std::clog<<"\t!!BUSTED!!"<<std::endl;
        }
    }else if(action == pause){
        return true;
    }else{
        std::clog<<"\tHOLD"<<std::endl;
        player->state = holding;
    }
    return false;
}

int Game::play(int rounds){
    int results = 0;  // for keeping track of player one wins
    for(int r = 1;r<=rounds;r++){
        // deal out cards
        for(int i=dealer.hand.size();i<2;i++) dealer.hand.add(deck.draw());
        dealer.state = playing;

        for(int p=0;p<players.size();p++){
            players[p]->state = playing;
            for(int i=players[p]->hand.size();i<2;i++){
                players[p]->hand.add(deck.draw());
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
                if(doAction(p)) return results;  // for pausing the game
            }
            dealer.showHand(1);
            doAction(&dealer);
            std::clog<<std::endl;
        }

        // determine winner
            // find highest player
        Gambler *currentMax = players[0];
        for(int i=1;i<players.size();++i){
            if(currentMax->hand.score() < players[i]->hand.score()){
                currentMax = players[i];
            }
        }
        // dealer one
        if(currentMax->hand.score()<=dealer.hand.score()){
            dealer.state = win;
            for (auto &&p : players){
                p->state = lose;
            }
        }else{ // check for ties
            for (auto &&p : players){
                if(p->hand.score() == currentMax->hand.score()){
                    p->state = win;
                }else{
                    p->state = lose;
                }
            }
            dealer.state = lose;
        }

        // print results
        results += players[0]->state==win?1:0;
        std::clog<<"Round "<<r<<" Results:"<<std::endl;
        std::clog<<"\tDealer:"<<coutState[dealer.state]<<std::endl;
        for (auto &&p : players){
            std::clog<<"\tPlayer:"<<p->ID<<":"<<coutState[p->state]<<std::endl;
        }
        reset();
    }
    return results;
}

int Game::hitme(){
    players[0]->hand.add(deck.draw());
    return players[0]->hand.score() != -1;
}

void Game::setPlayer(int i, Hand hand){
    for (auto &&card : hand.getCards()){
        deck.find(card.getID());
    }
    players[i]->hand = hand;
}