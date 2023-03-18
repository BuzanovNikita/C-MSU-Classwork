#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

struct sembuf upson1 = { 1, 1, 0 };
struct sembuf downson1 = { 1 , -1, 0 };
struct sembuf upson2 = { 2, 1, 0 };
struct sembuf downson2 = { 2 , -1, 0 };
struct sembuf upf = { 0, 1, 0 };
struct sembuf downf = { 0, -1, 0 };

int ch[2], data[2], semid;

void work_son( int sonnum ){
    read( ch[0], data, 2 * sizeof(int) );
    if ( data[0] < data[1] ) {
        write( ch[1], data, 2 * sizeof(int) );
        close( ch[0] );
        close( ch[1] );
        semctl( semid, 0, IPC_RMID, NULL);
        semop( semid, &upf, 1 );
        exit(0);
    }
    else{
        if ( sonnum == 2 ) printf("son2 %d %d\n", data[0], data[1]);
        else printf("son1 %d %d\n", data[0], data[1]);
        fflush(stdout);
        data[0]  = data[0] - 1;
        data[1] = data[1] + 1;
        write( ch[1], data, 2 * sizeof(int) );
        semop( semid, &upf, 1 );
    }
}

int main() {
    pid_t pid1, pid2;
    int sonnum = 1;

    key_t key = ftok("/etc", 'h');

    if ( key < 0 ) return 1;

    semid = semget( key, 3, 0664 | IPC_CREAT | IPC_EXCL );

    if ( semid < 0 ) return 1;

    if ( pipe( ch ) < 0 ) return 1;

    pid1 = fork();
    if ( pid1 > 0 ){//fsther
        pid2 = fork();
        if ( pid2 > 0 ){//father
            scanf( "%d %d", &data[0], &data[1] );
            write( ch[1], data, 2 * sizeof(int) );
            semop( semid, &upson1, 1 );
            while(1){
                semop( semid, &downf, 1 );
                read( ch[0], &data, 2 * sizeof(int) );
                if ( data[0] < data[1] ){
                    write( ch[1], &data, 2 * sizeof(int) );
                    semop( semid, &upson1, 1 );
                    semop( semid, &downf, 1 );
                    wait( NULL );
                    semop( semid, &upson2, 1 );
                    semop( semid, &downf, 1 );
                    wait( NULL );
                    exit(0);
                }
                else{
                    printf( "father %d %d\n", data[0], data[1] );
                    fflush( stdout );
                    data[0]--;
                    data[1]++;
                    write( ch[1], &data, 2 * sizeof(int) );
                    if ( sonnum ) semop( semid, &upson2, 1 );
                    else semop( semid, &upson1, 1 );
                    sonnum = 1 - sonnum;
                }
            }
        }
        if ( pid2 == 0 ){//son2
            while(1){
                semop( semid, &downson2, 1 );
                work_son (2);
            }
        }
        if ( pid2 < 0 ){
            fprintf( stderr, "%s", "fork_error\n" );
            close( ch[0] );
            close( ch[1] );
            semctl(semid, 0, IPC_RMID, NULL);
            exit(-1);
        }
    }
    if ( pid1 == 0 ){//son1
        while(1){
            semop( semid, &downson1, 1 );
            work_son (1);
        }
    }
    if ( pid1 < 0 ){
        fprintf( stderr, "%s", "fork_error\n" );
        close( ch[0] );
        close( ch[1] );
        semctl(semid, 0, IPC_RMID, NULL);
        exit(-1);
    }

    return 0;
}
