
all: main.o card.o deck.o players.o game.o
	mpic++ main.o game/deck.o game/card.o game/players.o game/game.o -o main.out

main.o: main.cpp
	mpic++ -c main.cpp

card.o: game/card.hpp game/card.cpp 
	cd game && \
	mpic++ -c card.cpp

deck.o: game/deck.hpp game/deck.cpp 
	cd game && \
	mpic++ -c deck.cpp

players.o: game/players.hpp game/players.cpp 
	cd game && \
	mpic++ -c players.cpp

game.o: game/game.hpp game/game.cpp 
	cd game && \
	mpic++ -c game.cpp

clean:
	rm game/*.o
	rm *.o *.out
