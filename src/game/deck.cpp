// #include <vector>
#include "deck.hpp"
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <vector>       // std::vector
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

Hand::Hand(Card card){
    cards.push_back(card);
}
Hand::Hand(std::vector<Card> cardList){
    for (auto &&c : cardList){
        cards.push_back(c);
    }
}
int Hand::size(){
    return cards.size();
}
Card Hand::getCard(int i){
    if(cards.size() > i && i>=0){
        return cards[i];
    }
    else{
        std::clog<<"ERR: Hand.getcard out of range, i:"<<i<<" of size:"<<cards.size()<<std::endl;
        return cards.back();
    }
}
std::vector<Card> Hand::getCards(){
    return cards;
}
int Hand::score(){
    int total = 0;
    int aceCount = 0;
    for (auto &&card : cards){
        if(card.score()>10){
            total += 0;
        }else{
            total += card.score();
            if(card.score()==1){
                aceCount ++;
            }
        }
    }
    while (total<12 && aceCount>0){
        total += 10;
        aceCount --;
    }
    return total;    
};
void Hand::add(Card card){
    cards.push_back(card);
}
void Hand::clear(){
    cards.clear();
}

//////////
// Deck
//////////

void Deck::init(){
    for(int i=0;i<numberOfDecks*52;i++){
        cards.push_back(Card(i));
    }
};
void Deck::shuffle(){
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(cards.begin(), cards.end(), e);
}

Deck::Deck(){
    numberOfDecks = 1;
    Deck::init();
    Deck::shuffle();
}

Deck::Deck(int n){
    numberOfDecks = n;
    Deck::init();
    Deck::shuffle();
}

Deck::Deck(int* data){
    // 0:numDecks, 1-52 number of cards 
    numberOfDecks = data[0];
    Deck::init();
    Deck::shuffle();
    // info = data
    for(int i=1;i<=52;i++){
        for(int j=0;j<data[i];j++){
            auto iter = std::find(cards.begin(),cards.end(),i-1);
            if(iter!=cards.begin()){
                info[i-1]++;
                cards.erase(iter);
            }else{
                std::clog<<"ERR: Deck initializer for card id:"<<i<<" of:"<<j<<"/"<<info[j]<<std::endl;
            }
        }
    }          
}
int* Deck::getInfo(){
    int *temp = new int[53];
    temp[0] = numberOfDecks;
    for(int i=1;i<=52;i++){
        temp[i] = info[i-1];
    }
    return temp;
}

Card Deck::draw(){
    Card temp = cards.back();
    info[temp.getID()%52]++;
    cards.pop_back();
    return temp;
}

Card Deck::find(int id){
    int index = -1;
    for(int i=0;i<cards.size();i++){
        if(cards[i].getID()%52==id){
            index = i;
            break;
        }
    }
    if(index==-1){
        std::clog<<"ERROR: Deck find :"<<id<<" not found"<<std::endl;
        return Card(id);
    }else{
        auto temp = cards[index];
        cards.erase(cards.begin()+index);
        return temp;
    }
}

int Deck::size(){
    return cards.size();
}