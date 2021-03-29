#pragma once

#include <iostream>
#include <string>

class Card{
    private:
        int ID;
    public:
        Card(int id);
        int getID();
        int score();
        std::string getRank();
        std::string getSuit();
        std::string str();
        bool operator==(Card rhs) const { return ID == rhs.getID();}
        friend std::ostream& operator<<(std::ostream& os, Card& card);
};