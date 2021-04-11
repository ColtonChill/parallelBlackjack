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
        void constructor(int n, bool a);
    public:
        std::vector<Player*> players;
        int automatic = 0; // default is manual
        int aiType = 3;
        Game(){constructor(1,false);}
        Game(int n){constructor(n,false);}  // first player is the human, all other are npc
        Game(bool a){constructor(1,a);}
        Game(int n, bool a){constructor(n,a);}
        Game(int* info);  // to be populated by the MPI packate
        int* gameInfo();
        Gamestate makeGameState();
        int play(int rounds);
        int play(){return play(1);}; // play without redealing cards, human first
        int hitme();
        void setPlayer(int i, Hand hand);
        void setPlayerAI(int i, Logic ai){players[i]->ai=ai;}
        void setPlayerAI(int i, int ai){aiType=ai;players[i]->ai=mapLogic();}
};
