#pragma once
#include "players.hpp"


class Game{
    private:
        Dealer dealer;
        Deck deck;
        void reset();
        bool isAnyonePlaying();
        bool doAction(Gambler *player);
        Logic mapLogic();
    public:
        std::vector<Player*> players;
        int automatic = 0; // default is manual
        int aiType = 3;
        Game(int numPlayers);  // first player is the human, all other are npc
        Game(bool a):Game(1){automatic = a;}
        Game(int n, bool a):Game(n){automatic = a;}
        Game():Game(1){}
        Game(int* info);  // to be populated by the MPI packate
        int* gameInfo();
        Gamestate makeGameState();
        int play(int rounds);
        int play(){return play(1);}; // play without redealing cards, human first
        int hitMe();
        void setAuto(bool b){automatic = b?1:0;}
        void setPlayer(int i, Hand hand);
};
