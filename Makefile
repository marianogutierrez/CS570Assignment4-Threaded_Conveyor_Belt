#Cameron Ozatalar
#Mariano Gutierrez

#Create Executable
mizzo: Main.o 
	g++ Main.o -g -o mizzo

#Create Main object file
Main.o: Main.cpp Producer.o
	g++ -c Main.cpp -lpthread -lrt

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
	rm Main.o Belt.o Producer.o Consumer.o mizzo