#Cameron Ozatalar
#Mariano Gutierrez

#Create Executable
mizzo: Main.o Belt.o
	g++ Main.o Belt.o -g -o mizzo

#Create Main object file
Main.o: Main.cpp
	g++ -c Main.cpp

#Create Belt object file
Belt.o: Belt.hpp Belt.cpp
	g++ -c Belt.cpp

#Clean up files
clean:
	rm Main.o mizzo