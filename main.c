#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("ERROR: You must spesify at least one microservice to run\n");
        exit(-1);
    }
    printf("this is first microservice: %s\n", argv[1]);
    return 0;
}