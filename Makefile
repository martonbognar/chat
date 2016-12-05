main: main.cpp client.cpp connector.cpp
	g++ -std=c++14 -pthread -Wall -Wextra -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wformat=2 -Wunreachable-code -pedantic *.cpp -o main -lsfml-network -lsfml-system
