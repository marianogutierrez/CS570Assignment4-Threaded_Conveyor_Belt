#Cameron Ozatalar
#Mariano Gutierrez

#Create Executable
mizzo: Main.cpp Producer.o Belt.o Consumer.o
	g++  Main.cpp Producer.o Belt.o Consumer.o  -g -o mizzo -lpthread -lrt

#Create Producer object file
Producer.o: Belt.o Producer.hpp Producer.cpp 
	g++ -c Producer.cpp

#Create Consumer object file
Consumer.o: Belt.o Consumer.hpp Consumer.cpp
	g++ -c Consumer.cpp

#Create Belt object file
Belt.o: Belt.hpp Belt.cpp
	g++ -c Belt.cpp

#Clean up files
clean:
	rm Belt.o Producer.o Consumer.o mizzo