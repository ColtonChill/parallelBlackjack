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
        //getters
        int size();
        Card getCard(int i);
        std::vector<Card> getCards();
        //else
        int score();
        void add(Card card);
        void clear();
};
class Deck{
    private:
        int numberOfDecks = 1;  // 1=52, 2=104, 3=156, 4=208
        std::vector<Card> cards;
        std::vector<int> info = std::vector<int>(52);
    public:
        Deck();
        Deck(int n);
        Deck(int* data);  // used to preload the dead
        int* getInfo(); // returns the info needed to recreate similar decks
        int size(); // number of cards, NOT numer of subdecks in deck
        void shuffle();
        void init();
        Card draw();
        Card find(int id);
};