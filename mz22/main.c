#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char* er = "Error";
int i=1;

void printfson( int N, int i ){
    int status, pid;
    pid = fork();
    if (pid>0){//father
        wait( &status );
        if ( N>i ) {
            i++;
            printfson( N, i );
        }
    }
    else if (pid==0){//son
        printf( "son %d\n", i );
        fflush( stdout );
        dup2( 2, 1 );
        printf( "PID = %d PPID = %d\n", getpid(), getppid() );
        fflush( stderr );
    }
    else{
        dup2( 2, 1 );
        printf( "%s\n", er );
    }
}

int main() {
    int N;
    scanf( "%d", &N );
    printfson( N, i );
    return 0;
}
