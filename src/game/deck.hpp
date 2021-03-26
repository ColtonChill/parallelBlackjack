#pragma once
#include <vector>
#include <string>
#include "card.hpp"

class Hand{
    private:
        std::vector<Card> cards;
    public:
        Hand() = default;
        Hand(Card card);
        Hand(std::vector<Card> cardList);
        int score();
        void add(Card card);
        void clear();
};
class Deck{
    private:
        std::vector<Card> cards;
    public:
        Deck(){Deck(1);}
        Deck(int n);
        Deck(int* info);  // used to preload the dead
};