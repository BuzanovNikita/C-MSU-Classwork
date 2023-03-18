#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

void Clean( int semid, int shmid, pid_t pida, pid_t pidA, pid_t pid1, int error ){
    shmctl( shmid, IPC_RMID, NULL );
    semctl(semid, 0, IPC_RMID, NULL);
    if ( pida > 0 ) kill( pida, SIGKILL );
    if ( pidA > 0 ) kill( pidA, SIGKILL );
    if ( pid1 > 0 ) kill( pid1, SIGKILL );
    wait(NULL);
    wait(NULL);
    wait(NULL);
    if (error) exit(-1);
    else exit(0);
}

int main() {
    struct sembuf upsona = { 1, 1, 0 };
    struct sembuf downsona = { 1 , -1, 0 };
    struct sembuf upsonA = { 2, 1, 0 };
    struct sembuf downsonA = { 2 , -1, 0 };
    struct sembuf upson1 = { 3, 1, 0 };
    struct sembuf downson1 = { 3 , -1, 0 };
    struct sembuf upf = { 0, 1, 0 };
    struct sembuf downf = { 0, -1, 0 };
    int n, i=0, j;
    pid_t pida=0, pid1=0, pidA=0;
    key_t key = ftok( ".", 9 );
    if ( key == -1 ){
        perror( "ftok_error" );
        exit(-1);
    }
    scanf( "%d", &n );
    int shmid = shmget( key, n+1, IPC_CREAT | IPC_EXCL | 0666 );
    if ( shmid == -1 ){
        perror( "shmget_error" );
        exit(-1);
    }
    char* shmaddr = shmat( shmid, NULL, 0 );

    key_t keysem = ftok( ".", 10 );
    if ( keysem == -1 ){
        shmctl( shmid, IPC_RMID, NULL );
        perror( "ftok_error" );
        exit(-1);
    }

    int semid = semget( key, 4, IPC_CREAT | IPC_EXCL | 0666);
    if ( semid == -1 ){
        shmctl( shmid, IPC_RMID, NULL );
        perror( "semget_error" );
        exit(-1);
    }

    shmaddr[n] = '\0';
    for ( i=0; i<n+1; i++ ) shmaddr[i] = '!';
    i = 0;

    pida = fork();
    if ( pida > 0 ){

        pidA = fork();

        if ( pidA > 0 ){

            pid1 = fork();

            if ( pid1 > 0 ){//father

                semop( semid, &upsona, 1 );
                semop( semid, &downf, 1 );
                printf(  "%s\n", shmaddr );
                Clean( semid, shmid, pida, pidA, pid1, 0 ); //ending

            }
            if ( pid1 == 0 ){//son1

                shmaddr = shmat( shmid, NULL, 0 );
                shmaddr[n] = '\0';

                while(1){

                    semop( semid, &downson1, 1 );
                    j = 0;
                    while( (shmaddr[j] != '!') && (shmaddr[j] != '\0') ) j++;

                    if ( shmaddr[j] == '\0' ) {
                        semop( semid, &upf, 1 );
                        exit(0);
                    }

                    shmaddr[j] = '0' + (i % 10);
                    i++;
                    semop( semid, &upsonA, 1 );

                }

            }
            if ( pid1 < 0 ) Clean( semid, shmid, pida, pidA, pid1, 1 ); //error

        }
        if ( pidA == 0 ){//sonA

            shmaddr = shmat( shmid, NULL, 0 );
            shmaddr[n] = '\0';

            while(1){

                semop( semid, &downsonA, 1 );
                j = 0;
                while( (shmaddr[j] != '!') && (shmaddr[j] != '\0') ) j++;

                if ( shmaddr[j] == '\0' ) {
                    semop( semid, &upf, 1 );
                    exit(0);
                }

                shmaddr[j] = 'A' + (i % 26);
                i++;
                semop( semid, &upsona, 1 );

            }

        }
        if ( pidA < 0 ) Clean( semid, shmid, pida, pidA, pid1, 1 ); //error

    }
    if ( pida == 0 ){//sona

        shmaddr = shmat( shmid, NULL, 0 );
        shmaddr[n] = '\0';

        while(1){

            semop( semid, &downsona, 1 );
            j = 0;
            while( (shmaddr[j] != '!') && (shmaddr[j] != '\0') ) j++;

            if ( shmaddr[j] == '\0' ) {
                semop( semid, &upf, 1 );
                exit(0);
            }

            shmaddr[j] = 'a' + (i % 26);
            i++;
            semop( semid, &upson1, 1 );

        }

    }
    if ( pida < 0 ) Clean( semid, shmid, pida, pidA, pid1, 1 ); //error

    return 0;
}
