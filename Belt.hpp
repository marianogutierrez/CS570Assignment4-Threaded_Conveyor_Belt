#include <queue>

//Cameron Ozatalar
//Mariano Gutierrez

class Belt{
    public:
        std::queue<int> *belt;
        int total;
        int crunFrogNum;
        int escargotNum;
        Belt();
        ~Belt();
        void push(int itemToQueue);
        int pop();
};