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
//function prototypes
void* consumeCandy(void* worker);
void* makeEscargot(void* producer);
void* makeFrogBites(void* producer);
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
    Consumer* Lucy = new Consumer(conveyerBelt, "Lucy"); // need to have the name to to figure out who did what!
    Consumer* Ethel = new Consumer(conveyerBelt, "Ethel"); // of course both workers are on the same belt!
    Producer* escargotProducer = new Producer(conveyerBelt, escargotSpeed);
    Producer* frogBiteProducer = new Producer(conveyerBelt, crunFrogSpeed);

    pthread_t lucy,ethel,frogBiteGen,escargotGen; 
    pthread_create(&frogBiteGen,NULL,makeFrogBites,(void*) frogBiteProducer) ;
    pthread_create(&escargotGen,NULL,makeEscargot,(void *) escargotProducer);
    pthread_create(&lucy,NULL,consumeCandy, Lucy);
    pthread_create(&ethel, NULL,consumeCandy, Ethel);

    // join up *might not need*
    pthread_join(frogBiteGen, NULL);
    pthread_join(escargotGen, NULL);
    pthread_join(lucy, NULL);
    pthread_join(ethel, NULL);
    
    
    // main thread block untill consumption of 100th candy 
    sem_wait(&(conveyerBelt ->barrier));

    //Print report, free memory, and exit with success
    Lucy ->totalConsumed = Lucy->escargotConsumed + Lucy ->frogBiteConsumed;
    Ethel ->totalConsumed = Ethel->escargotConsumed + Ethel ->frogBiteConsumed;
    cout << "PRODUCTION REPORT" << endl;
    cout << "----------------------------------------" << endl;
   // cout << "crunchy frog bite producer generated " << conveyerBelt -> gencrunFrogNum << " candies" << endl;
    //cout << "escargot sucker producer generated " << conveyerBelt -> genescargotNum << " candies" << endl;
    cout << "Lucy consumed " << Lucy ->frogBiteConsumed << " crunchy frog bites + ";  
    cout << Lucy ->frogBiteConsumed << " escargot suckers = " << Lucy -> totalConsumed << endl;
    cout << "Ethel consumed " << Ethel -> frogBiteConsumed << " crunchy frog bites + ";
    cout << Ethel ->escargotConsumed << " escargot suckers = "  << Ethel -> totalConsumed << endl;

    delete Lucy;
    delete Ethel;
    delete escargotProducer;
    delete frogBiteProducer;
    delete conveyerBelt;
    exit(0); // success code
}

//consumer processes must share common code 
void* consumeCandy(void* worker) {
    Consumer *consume = (Consumer*) worker; // safe, I know what I'm dealing with
    string name = consume -> name; // quick ref;
    for(;;) {
      sem_wait(&(consume -> conveyor -> consumeKey)); // lock, till stuff to grab is avail
      while(consume ->conveyor ->belt ->size != 0) {
          int candy = consume ->conveyor ->pop();
          if(candy == escargotSucker) {
              consume ->escargotConsumed++;
              cout << name << "consumed crunchy frog bite."
          }
          else { // it's a frog bite 
            consume ->frogBiteConsumed++;
            cout << name << "consumed crunchy frog bite."
          }
      }
     if(consume ->conveyor->lifeTimeProduced == maxCandy) { // exit case
        sem_post(&(consume -> conveyor ->barrier)); // open the barrier
        break;
       }
      sem_post(&(consume -> conveyor -> produceKey));
   }
}

//producers should stop once 100 are made in total 
void* makeFrogBites(void* producer) {
    Producer* produce = (Producer*) producer;
    for(;;) {
        sem_wait(&(produce -> conveyor -> produceKey)); // lock. Entering crit section
        if(produce -> conveyor -> lifeTimeProduced == maxCandy) { // 100 for the day
            sem_post(&(produce -> conveyor ->consumeKey)); // relinquish lock
            break; // outta here
        }
        while(produce -> conveyor -> push(crunFrogBites)) { //while we can push more candy
            cout << "Belt: " << produce -> conveyor -> frogs << " frogs + ";
            cout << "escargots = " << produce ->conveyor->escargots; 
            cout << "    " << "Added crunchy frog bite.";
            if(produce -> conveyor ->frogs == 3) { // the max num allowed at a time is 3!
                sem_post(&(produce ->conveyor->produceKey)); // relinquish lock to to other maker
                break; // back out and wait your turn once more!
            }
            //otherwise sleep the amt given
            sem_post(&(produce -> conveyor ->consumeKey)); // there's stuff on the belt go get it!
            sem_wait(&(produce ->conveyor->produceKey)); // relinquish lock to to other maker
            sleep(produce -> speed/1000); // /1000 to get ms
        }
        sem_post(&(produce -> conveyor ->consumeKey)); // out of while relinquish lock
    } 
}

void* makeEscargot(void* producer) {
    Producer* produce = (Producer*) producer; // safe, I know what I'm dealing with
    for(;;) {
        sem_wait(&(produce -> conveyor -> produceKey)); // lock Entering critical section
        if(produce -> conveyor -> lifeTimeProduced == maxCandy) { // 100 for the day
            sem_post(&(produce -> conveyor ->consumeKey)); // relinquish lock
            break; // outta here
        }
        //returns true as long as there are no more than 10 candies on the belt!
        while(produce -> conveyor -> push(escargotSucker)) {
            cout << "Belt: " << produce -> conveyor -> frogs << " frogs + ";
            cout << "escargots = " << produce ->conveyor->escargots; 
            cout << "    " << "Added escargot sucker.";
            sem_post(&(produce -> conveyor ->consumeKey)); // there's stuff on the belt go get it!
            sem_wait(&(produce -> conveyor -> produceKey)); // other producer gets a turn
            sleep(produce ->speed/1000); // /1000 to get ms
        }
        // out of while meaning ten items on the belt
        sem_post(&(produce -> conveyor ->consumeKey)); // relinquish lock
    }
}