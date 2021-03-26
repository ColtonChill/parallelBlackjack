
all: main.o game.o
	g++ main.o game/game.o -o main.out

main.o: main.cpp
	g++ -c main.cpp

game.o: game/card.hpp game/card.cpp
	cd game && \
	g++ -c card.cpp -o game.o

clean:
	rm *.o
	rm game/*.o