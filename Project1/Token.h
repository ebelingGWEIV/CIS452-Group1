
#ifndef LAB6_PROCESSNODE_H
#define LAB6_PROCESSNODE_H

struct token{
    int dest;
    int src;
    char message[128]; //This may need to be changed out for a pointer to shared memory.
};


#endif //LAB6_PROCESSNODE_H
