g++ -c -o Buddhabrot.o abb.cpp -O3
g++ -o Buddhabrot Buddhabrot.o -Wall -fopenmp -lpthread -lm -lX11 -lsfml-graphics -lsfml-window -lsfml-system
$SHELL