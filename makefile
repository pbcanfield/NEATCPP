NEAT:	NetworkManager.o Genome.o NeuralNetwork.o Node.o Weight.o main.o
	g++ -Wall -g -std=c++14 -pthread obj/Genome.o obj/Node.o obj/NeuralNetwork.o obj/NetworkManager.o obj/Weight.o  obj/main.o -lsfml-graphics -lsfml-window -lsfml-system -o exec/NEAT

main.o:	src/main.cpp
	g++ -Wall -g -std=c++14 -pthread -c src/main.cpp -o obj/main.o

NetworkManager.o:	src/NetworkEvolution/NetworkManager.cpp src/NetworkEvolution/NetworkManager.h
	g++ -Wall -g -std=c++14 -pthread -c src/NetworkEvolution/NetworkManager.cpp -o obj/NetworkManager.o

Genome.o:	src/NetworkEvolution/NeuralNetwork/Genome.cpp src/NetworkEvolution/NeuralNetwork/Genome.h src/NetworkEvolution/NeuralNetwork/Gene.h
	g++ -Wall -g -std=c++14 -pthread -c src/NetworkEvolution/NeuralNetwork/Genome.cpp -o obj/Genome.o

NeuralNetwork.o:	src/NetworkEvolution/NeuralNetwork/NeuralNetwork.cpp src/NetworkEvolution/NeuralNetwork/NeuralNetwork.h
	g++ -Wall -g -std=c++14 -pthread -c src/NetworkEvolution/NeuralNetwork/NeuralNetwork.cpp -o obj/NeuralNetwork.o

Node.o:	src/NetworkEvolution/NeuralNetwork/Node.cpp src/NetworkEvolution/NeuralNetwork/Node.h
	g++ -Wall -g -std=c++14 -pthread -c src/NetworkEvolution/NeuralNetwork/Node.cpp -o obj/Node.o

Weight.o:	src/NetworkEvolution/NeuralNetwork/Weight.cpp src/NetworkEvolution/NeuralNetwork/Weight.h
	g++ -Wall -g -std=c++14 -pthread -c src/NetworkEvolution/NeuralNetwork/Weight.cpp -o obj/Weight.o

clean:
	rm -rf obj/* exec/*
