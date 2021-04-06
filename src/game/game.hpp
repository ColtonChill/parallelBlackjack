#pragma once
#include "players.hpp"


class Game{
    private:
        std::vector<Player*> players;
        Dealer dealer;
        Deck deck;
        void reset();
        bool isAnyonePlaying();
        bool doAction(Gambler *player);
        Logic mapLogic();
    public:
        int automatic = 0; // default is manual
        int aiType = 3;
        Game(int numPlayers);  // first player is the human, all other are npc
        Game(bool a):Game(1){automatic = a;}
        Game(int n, bool a):Game(n){automatic = a;}
        Game():Game(1){}
        Game(int* info);  // to be populated by the MPI packate
        int* gameInfo();
        Gamestate makeGameState();
        void play(int rounds, bool continuing);
        void play(int rounds){play(rounds,false);};
        void play(){play(1);}; // play without redealing cards, human first
        void setAuto(bool b){automatic = b;}
};