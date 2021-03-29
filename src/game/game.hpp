#pragma once
#include "players.hpp"


class Game{
    private:
        std::vector<Player*> players;
        Dealer dealer;
        Deck deck;
        void reset();
        bool isAnyonePlaying();
        void doAction(Gambler *player);
    public:
        Game(int numPlayers);
        Game(int * info);  // to be populated by the MPI packate
        void play();
};