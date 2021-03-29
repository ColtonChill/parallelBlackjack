
all: main.o card.o deck.o players.o game.o
	g++ main.o game/deck.o game/card.o -o main.out

main.o: main.cpp
	g++ -c main.cpp

card.o: game/card.hpp game/card.cpp 
	cd game && \
	g++ -c card.cpp

deck.o: game/deck.hpp game/deck.cpp 
	cd game && \
	g++ -c deck.cpp

players.o: game/players.hpp game/players.cpp 
	cd game && \
	g++ -c players.cpp

game.o: game/game.hpp game/game.cpp 
	cd game && \
	g++ -c game.cpp

clean:
	rm *.o
	rm game/*.o