#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int pipes[2];
pid_t pid1, pid2;
pid_t target_pid, ist_pid;
int ball1, ball2, flag = 0;
char son1[] = "son1 ";
char son2[] = "son2 ";
char fatger[] = "father ";
char sp[] = " ";
char nl[] = "\n";


void SigHndl ( int signum ){

    signal( SIGUSR1, SigHndl );
    read( pipes[0], &ball1, sizeof(int) );
    read( pipes[0], &ball2, sizeof(int) );


    if ( ball1 >= ball2 ){

        if ( ist_pid == getppid() ){//son2
            flag = 1;
        }
        else{
            if ( target_pid == getppid() ){//son1
                flag = 1;
            }
            else{//father
                flag = 1;
            }
        }

    }
    else{

        if ( (ist_pid != getppid()) && (target_pid != getppid()) ){//father
            close( pipes[0] );
            close( pipes[1] );
            kill( pid1, SIGUSR2 );
            kill( pid2, SIGUSR2 );
            wait( NULL );
            wait( NULL );
            exit(0);
        }
        else{//son
            write( pipes[1], &ball1, sizeof(int) );
            write( pipes[1], &ball2, sizeof(int) );
            kill( getppid(), SIGUSR1 );
        }
    }
}

void finish( int signum ){
    close( pipes[0] );
    close( pipes[1] );
    exit(0);
    signal( SIGUSR2, finish );
}

int main(){
    int er;
    er = pipe( pipes );
    if ( er == -1 ) exit(-1);
    signal( SIGUSR1, SigHndl );
    signal( SIGUSR2, finish );

    pid1 = fork();
    if ( pid1 > 0 ){//father

        pid2 = fork();

        if ( pid2 > 0 ){//father

            scanf( "%d %d", &ball1, &ball2 );
            write( pipes[1], &ball1, sizeof(int) );
            write( pipes[1], &ball2, sizeof(int) );

            ist_pid = pid1;
            target_pid = pid2;
            kill( pid2, SIGUSR1 );
            for(;;) {
                if (flag) {
                    printf( "father %d %d\n", ball1, ball2 );
                    fflush( stdout );
                    flag = 0;
                    kill( target_pid, SIGUSR1 );

                    ball1--;
                    ball2++;

                    write( pipes[1], &ball1, sizeof(int) );
                    write( pipes[1], &ball2, sizeof(int) );

                }
            }

        }
        if( pid2 == 0) {//son2

            ist_pid = getppid();
            target_pid = pid1;
            for(;;) {
                if (flag) {
                    printf( "son2 %d %d\n", ball1, ball2 );
                    fflush( stdout );
                    flag = 0;
                    kill( target_pid, SIGUSR1 );

                    ball1--;
                    ball2++;

                    write( pipes[1], &ball1, sizeof(int) );
                    write( pipes[1], &ball2, sizeof(int) );
                }
            }


        }
        if( pid2 == -1 ) {//error

            kill( pid1, SIGUSR2 );
            wait( NULL );
            close( pipes[0] );
            close( pipes[1] );
            exit(-1);

        }

    }

    if ( pid1 == 0) {//son1

        //ist_pid = -1;
        target_pid = getppid();
        for(;;) {
            if (flag) {
                printf( "son1 %d %d\n", ball1, ball2 );
                fflush( stdout );
                flag = 0;
                kill( target_pid, SIGUSR1 );

                ball1--;
                ball2++;

                write( pipes[1], &ball1, sizeof(int) );
                write( pipes[1], &ball2, sizeof(int) );

            }
        }


    }

    if ( pid1 == -1 ) {//error

        close( pipes[0] );
        close( pipes[1] );
        exit(-1);

    }
    return 0;
}