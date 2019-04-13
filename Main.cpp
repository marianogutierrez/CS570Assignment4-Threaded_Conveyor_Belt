#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // threads
#include <unistd.h> // for the sleep functions in ms
#include <semaphore.h>
#include <string.h>
#include <getopt.h>
#include <iostream> // cout and the like
#include "Belt.hpp"
#include "Consumer.hpp"
#include "Producer.hpp"
// NOTE: Do not use global vars to comm to threads use data structs 
//Cameron Ozatalar
//Mariano Gutierrez
using namespace std;
//function prototypes:
void* consumeCandy(void* worker);
void* makeEscargot(void* producer);
void* makeFrogBites(void* producer);
void consumeDescripton(Consumer* consume, int total);

const int IllegalOP = 1; // illegal cmd line arg error
const int escargotSucker = 0; // constants to know of which candy
const int crunFrogBites = 1;
const int maxCandy = 100;

int main(int argc, char** argv){
    //Variables for getopt and default values:
    char flag = '\0'; // flag in command line
    int ethelSpeed = 0; //in milliseconds
    int lucySpeed = 0; //in milliseconds
    int crunFrogSpeed = 0; //in milliseconds
    int escargotSpeed = 0; //in milliseconds

    while((flag = getopt(argc, argv, "E:L:f:e:")) != -1){
        switch(flag){
            case 'E':
                ethelSpeed = atoi(optarg);
                break;
            case 'L':
                lucySpeed = atoi(optarg);
                break;
            case 'f':
                crunFrogSpeed = atoi(optarg);
                break;
            case 'e':
                escargotSpeed = atoi(optarg);
                break;
            default:
                //Invalid argument passed
                printf("Illegal option supplemented or missing arguements: %c\n", optopt);
                exit(IllegalOP); 
        }
    }

    //Print the speeds (debug)
    printf("Speeds:\n\tEthel Speed: %d\n\tLucy Speed: %d\n\tCrunchy Frog Speed: %d\n\tEscargot Speed: %d\n", 
    ethelSpeed, lucySpeed, crunFrogSpeed, escargotSpeed);

    // Our main workers
    Belt *conveyerBelt = new Belt(); // NOTE: NO MORE THAN 10 CANDIES ON THE BELT AT A TIME
    Producer* escargotProducer = new Producer(conveyerBelt, escargotSpeed);
    Producer* frogBiteProducer = new Producer(conveyerBelt, crunFrogSpeed);
    Consumer* Lucy = new Consumer(conveyerBelt,lucySpeed,"Lucy"); // need to have the name to to figure out who did what!
    Consumer* Ethel = new Consumer(conveyerBelt,ethelSpeed,"Ethel"); // of course both workers are on the same belt!

    pthread_t lucy,ethel,frogBiteGen,escargotGen; 
    //producers first 
    pthread_create(&escargotGen,NULL,makeEscargot,(void *) escargotProducer);
    pthread_create(&frogBiteGen,NULL,makeFrogBites,(void*) frogBiteProducer);
    pthread_create(&lucy,NULL,consumeCandy, Lucy);
    pthread_create(&ethel, NULL,consumeCandy, Ethel);

    // could detach too.
    //Note, the main thread is suspended untill the thread terminates/joined
    pthread_join(frogBiteGen, NULL);
    pthread_join(escargotGen, NULL); 
    pthread_join(lucy, NULL);
    pthread_join(ethel, NULL);
    
    // main thread block untill consumption of 100th candy 
    sem_wait(&(conveyerBelt ->barrier));

    //Print report, free memory, and exit with success
    Lucy -> totalConsumed = Lucy->escargotConsumed + Lucy ->frogBiteConsumed;
    Ethel -> totalConsumed = Ethel->escargotConsumed + Ethel ->frogBiteConsumed;
    cout << "PRODUCTION REPORT" << endl;
    cout << "----------------------------------------" << endl;
    cout << "crunchy frog bite producer generated " << frogBiteProducer->totalProduced << " candies" << endl;
    cout << "escargot sucker producer generated " << escargotProducer->totalProduced << " candies" << endl;
    cout << "Lucy consumed " << Lucy ->frogBiteConsumed << " crunchy frog bites + ";  
    cout << Lucy -> escargotConsumed << " escargot suckers = " << Lucy -> totalConsumed << endl;
    cout << "Ethel consumed " << Ethel -> frogBiteConsumed << " crunchy frog bites + ";
    cout << Ethel ->escargotConsumed << " escargot suckers = "  << Ethel -> totalConsumed << endl;

    delete Lucy;
    delete Ethel;
    delete escargotProducer;
    delete frogBiteProducer;
    delete conveyerBelt;
    exit(0); // success code
}

//shorten the code, by putting the output lines here
void consumeDescript(Consumer* consume, int total) {
    string name = consume -> name; // quick reference
    cout << "Belt: " << consume -> conveyor -> frogs << " frogs + " << "";
    cout << consume->conveyor->escargots << " escargots = " << total <<"."; 
    cout << " produced: " << consume ->conveyor->lifeTimeProduced << "   ";
}

//consumer processes must share common code 
void* consumeCandy(void* worker) {
    Consumer *consume = (Consumer*) worker; // safe cast, I know what I'm dealing with
    string name = consume -> name; // quick reference
    for(;;) {
      sem_wait(&(consume -> conveyor -> consumeSignal)); // block untill something to consume 
      sem_wait(&(consume -> conveyor -> mutex)); 
      // This starting if is to ensure other thread leaves.
      if(consume ->conveyor->lifeTimeConsumed == maxCandy) { // exit casephoeix
        break; // ... and be free
      }
      if(consume ->conveyor -> belt ->size() != 0) { // If I can get some candy then ->
          int candy = consume ->conveyor ->pop(); //  -> grab the candy and determine which it was!
          if(candy == escargotSucker) {
              consume ->escargotConsumed++;
              consume->conveyor->escargots--;
              int currentTotal = consume -> conveyor -> escargots + consume-> conveyor -> frogs;
              consumeDescript(consume,currentTotal);
              cout << name << " consumed escargot." << endl;
          }
          else { // it's a frog bite 
            consume ->frogBiteConsumed++;
            consume->conveyor->frogs--;
            int currentTotal = consume -> conveyor -> escargots + consume-> conveyor -> frogs;
            consumeDescript(consume,currentTotal);
            cout << name << " consumed crunchy frog bite." << endl;
            if(consume->conveyor->frogs == 2)  // there are no longer 3 on the belt
                sem_post(&(consume->conveyor->frogSignal)); // signal to the producer to make more
          }
      }
          // If that candy was the 100th...
          if(consume ->conveyor->lifeTimeConsumed == maxCandy) { // exit case
            sem_post(&(consume -> conveyor -> mutex)); // release mutex to free other thread outside.
            sem_post(&(consume -> conveyor ->barrier)); // open the barrier! 
            break; // ... and be free
          }
          else {
            sem_post(&(consume -> conveyor -> mutex)); // let the other one go 
            sem_post(&(consume -> conveyor -> availSlots)); // more slots ready
            usleep((consume ->delay) * 1000); // sleep between each, note div by 1000 to get ms 
          }
   }
}

//producers should stop once 100 are made in total 
void* makeFrogBites(void* producer) {
    Producer* produce = (Producer*) producer;
    for(;;) {
        sem_wait(&(produce -> conveyor -> availSlots)); // lock. Entering crit section
        sem_wait(&(produce -> conveyor -> mutex));
        if(produce -> conveyor -> lifeTimeProduced == maxCandy) { // 100 for the day
            sem_post(&(produce -> conveyor -> mutex)); // it has the lock, free it 
            sem_post(&(produce -> conveyor ->consumeSignal));
            break; // done other generator finished first 
        }
        
        if(produce -> conveyor -> push(crunFrogBites)) { //while we can push more candy
            produce -> conveyor ->frogs++;
            produce ->totalProduced++;
            int currentTotal = produce -> conveyor -> escargots + produce -> conveyor -> frogs;
            cout << "Belt: " << produce -> conveyor -> frogs << " frogs + ";
            cout <<  produce ->conveyor->escargots <<  " escargots = " << currentTotal << "."; 
            cout << " produced: " << produce ->conveyor->lifeTimeProduced;
            cout << "   " << "Added crunchy frog bite." << endl;
            if(produce -> conveyor -> lifeTimeProduced == maxCandy) { // 100 for the day
                sem_post(&(produce -> conveyor -> mutex));
                sem_post(&(produce -> conveyor ->consumeSignal));
                break;
            }
            else if(produce -> conveyor ->frogs == 3) { // the max num allowed at a time is 3!
                sem_post(&(produce -> conveyor -> mutex)); // let another do work
                sem_post(&(produce -> conveyor ->consumeSignal));
                sem_wait(&(produce->conveyor->frogSignal)); // block this thread 
                usleep((produce -> speed) * 1000); // /1000 to get ms
            } 
            else {
                sem_post(&(produce -> conveyor -> mutex));
                sem_post(&(produce -> conveyor ->consumeSignal));
                usleep((produce -> speed) * 1000); // /1000 to get ms
            }
        }
    } 
}

void* makeEscargot(void* producer) {
    Producer* produce = (Producer*) producer; // safe, I know what I'm dealing with
    for(;;) {
        sem_wait(&(produce -> conveyor -> availSlots)); 
        sem_wait(&(produce -> conveyor -> mutex));
        // ensures thread leaves if other generator finsihed. i.e.
        // what if the thread was outside? waiting? The other generator was responsible!
        if(produce -> conveyor -> lifeTimeProduced == maxCandy) {
            sem_post(&(produce -> conveyor -> mutex)); // it has the mutex so free it
            sem_post(&(produce -> conveyor ->consumeSignal)); // what if the other consumer
            break;
         }
        //returns true as long as there are no more than 10 candies on the belt!
        if(produce -> conveyor -> push(escargotSucker)) {
            produce -> conveyor ->escargots++;
            produce ->totalProduced++;
            int currentTotal = produce -> conveyor -> escargots + produce -> conveyor -> frogs;
            cout << "Belt: " << produce -> conveyor -> frogs << " frogs + ";
            cout <<  produce ->conveyor->escargots << " escargots = " << currentTotal <<  "."; 
            cout << " produced: " << produce ->conveyor->lifeTimeProduced;
            cout << "   " << "Added escargot sucker." << endl;
            if(produce -> conveyor -> lifeTimeProduced == maxCandy) {
                sem_post(&(produce -> conveyor -> mutex));
                sem_post(&(produce -> conveyor ->consumeSignal));
                break; // this generator was responsible. End it.
            }
            sem_post(&(produce -> conveyor -> mutex));
            sem_post(&(produce -> conveyor ->consumeSignal));
            usleep((produce ->speed) * 1000); // /1000 to get ms
        }
    }
}