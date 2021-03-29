
all: main.o card.o deck.o
	g++ main.o game/deck.o game/card.o -o main.out

main.o: main.cpp
	g++ -c main.cpp

card.o: game/card.hpp game/card.cpp 
	cd game && \
	g++ -c card.cpp

deck.o: game/deck.hpp game/deck.cpp 
	cd game && \
	g++ -c deck.cpp

clean:
	rm *.o
	rm game/*.o