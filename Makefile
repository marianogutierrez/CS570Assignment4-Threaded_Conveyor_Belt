#Cameron Ozatalar

#Create Executable
mizzo: Main.o
	g++ Main.o -g -o mizzo

#Create Main object file
Main.o: Main.cpp
	g++ -c Main.cpp

clean:
	rm Main.o mizzo