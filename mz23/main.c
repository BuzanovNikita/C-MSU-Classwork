#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char* er = "Error";
int i=1;

void printfpids( int N, int pid, int i ){
    int tmp;
    if ( i > N ) return;
    tmp = fork();
    if ( tmp == 0 ) {
        printf( "%d %d\n", getpid()-getppid(), getpid()-pid );
        fflush( stdout );
        fprintf( stderr, "PID = %d PPID = %d\n", getpid(), getppid() );
        fflush( stderr );
        i++;
        printfpids( N, pid, i );
    }
    if ( tmp > 0 ){
        wait(NULL);
    }
    if ( tmp < 0 ){
        dup2(2,1);
        printf( "%s", er );
        return;
    }
    return;
}

int main() {
    int N, pid=getpid();
    scanf( "%d", &N );
    printfpids( N, pid, i );
    return 0;
}
