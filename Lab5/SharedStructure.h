#ifndef CIS452_GROUP1_SHAREDSTRUCTURE_H
#define CIS452_GROUP1_SHAREDSTRUCTURE_H

#define FOO 4096
// Used to interpret the shared memory location
#define NUM_READERS 2
#define MESSAGE_LEN 25
struct messageStruct{
    int readers[NUM_READERS];
    char message[MESSAGE_LEN];
};

#endif //CIS452_GROUP1_SHAREDSTRUCTURE_H
