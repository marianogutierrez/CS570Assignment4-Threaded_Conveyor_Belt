#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

//Cameron Ozatalar

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
                exit(1);
        }
    }

    //Print the speeds
    printf("Speeds:\n\tEthel Speed: %d\n\tLucy Speed: %d\n\tCrunchy Frog Speed: %d\n\tEscargot Speed: %d\n", ethelSpeed, lucySpeed, crunFrogSpeed, escargotSpeed);

    exit(0);
}