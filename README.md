# Threaded Conveyor Belt
This program is loosely based off this famous scene: https://www.youtube.com/watch?v=HnbNcQlzV-4. Essentially, we have candies coming down a conveyor and our threaded program processes each one. The threads are controlled via semaphores and there system calls like init, post, and wait which are used extensivley. The program for example stops once 100 candies have been processed, and there cannot be more than 10 candies on the belt at the same time. All these constraints are handled by particular posts and waits. 

## Running the program
The program includes a makefile, so typing 
```
make mizzo 
```
is all that is required. Some optional parameters like -e can be inputted to change the speed at which candies are produced or consumed. 

```
-E for ethel speed (consumer thread)
-L for Lucy speed (consumer thread)
-f "frogbite" candy speed (producer)
-e "escargot" candy speed (producer) 
```

