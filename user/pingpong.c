#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argCount, char *argValues[])
{
    int p2c[2], c2p[2];
    int procID;



    if(argCount != 1){
        printf("error!\n");
        exit(1);
    }

    pipe(p2c);
    pipe(c2p);

    if(pipe(p2c) == -1){
        printf("create pipe from parent to child fail.");
        exit(1);
    }
    if(pipe(c2p) == -1){
        printf("create pipe from child to parent fail.");
        exit(1);
    }

    procID = fork();

   if(procID == 0){ //child
    close(p2c[1]);
    close(c2p[0]);

    char byte;
    read(p2c[0], &byte, 1);

    printf("%d: received ping\n", getpid());

    write(c2p[1], &byte, 1);
    exit(0);
   }
   else {
    close(p2c[0]);
    close(c2p[1]);

    char byte='a';

    write(p2c[1], &byte, 1);

    read(c2p[0], &byte, 1);
    
    printf("%d: received pong\n", getpid());
   }
   exit(0);
}