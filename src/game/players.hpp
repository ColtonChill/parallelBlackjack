#pragma once
#include <string>
#include "deck.hpp"

enum State {playing, busted, holding, win, lose};
enum Turn {hit, hold};

class Gambler{
    public:
        Hand hand;
        State state;
        std::string name;
        void reset();
        virtual Turn takeTurn();
        virtual void showHand();
};

class Dealer:public Gambler{
    public:
        Dealer();
        Turn takeTurn();
        void showHand();
};

class Player:public Gambler{
    public:
        Player(std::string s){name=s;}
        void showHand();
};

class Human:public Player{
    private:
    public:
        Human(std::string s):Player(s){}
        Turn takeTurn();
};

class NPC:public Player{
    public:
        NPC(std::string s):Player(s){}
        Turn takeTurn();
    //need to add more logic stuff
};