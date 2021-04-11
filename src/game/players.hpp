#pragma once
#include <string>
#include "deck.hpp"

enum State {playing=0, busted, holding, win, lose};
enum Turn {hit, hold, pause};

typedef std::pair<std::vector<Hand>,Deck> Gamestate;
typedef Turn (*Logic)(int id, Gamestate g);

//strategies
Turn hitMe(int id, Gamestate gamestate); // 0
Turn passMe(int id, Gamestate gamestate); // 1
Turn manuel(int id, Gamestate gamestate); // 2
Turn basic(int id, Gamestate gamestate); // 3
Turn cardCounting(int id, Gamestate gamestate); // 4
Turn psychic(int id, Gamestate gamestate); // 5

// players
class Gambler{
    public:
        Logic ai;
        Hand hand;
        State state;
        int ID;
        Gambler(int id);  // player label
        Gambler(int id, int* handInfo);
        Gambler(int id, Logic l);
        Gambler(int id, Logic l, int* handInfo);
        void reset();  // clears hand
        virtual ~Gambler(){}
        virtual Turn takeTurn(Gamestate gamestate){return ai(ID,gamestate);}
        virtual void showHand(bool title)=0;
};

class Dealer:public Gambler{
    public:
        Dealer():Gambler(0){}
        Dealer(int* handInfo):Gambler(0, handInfo){}
        ~Dealer(){};
        void showHand(bool title);
};

class Player:public Gambler{
    public:
        Player(int id):Gambler(id){}
        Player(int id, Logic l):Gambler(id,l){}
        Player(int id, int* handInfo):Gambler(id, handInfo){}
        Player(int id, Logic l, int* handInfo):Gambler(id, l, handInfo){}
        void showHand(bool title);
};
