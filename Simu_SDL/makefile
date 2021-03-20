
main: main.o 
		g++ -o main -Wall -I/usr/include/SDL2 main.cpp -lSDL2_image -lSDL2_ttf -lSDL2

main.o: main.cpp
		g++ -c main.cpp -Wall -g

clean :
	rm -f *.o 	

