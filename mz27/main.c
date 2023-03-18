#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>


void kill_them_all( int msgid, pid_t pida, pid_t pidb, pid_t pidc ){
    kill( pida, SIGKILL );
    kill( pidb, SIGKILL );
    kill( pidc, SIGKILL );
    wait( NULL );
    wait( NULL );
    wait( NULL );
    msgctl( msgid, IPC_RMID, NULL );
    exit(0);
}

int main() {
    pid_t pida, pidb, pidc;
    unsigned int class, x1, x2, x3;
    unsigned int tmp, res;
    struct msgbuf {
        long mtype;
        unsigned char mtext[4];
    } msg;
    key_t key = ftok( ".", 23 );
    if ( key == -1 ) return -1;
    int msgid = msgget( key, IPC_CREAT | IPC_EXCL | 0666 );
    if ( msgid == -1 ) return -1;

    pida = fork();
    if ( pida > 0 ){//father

        pidb = fork();

        if ( pidb > 0 ){//father

            pidc = fork();
            if ( pidc > 0 ){//father

                while( scanf( "%u.%u.%u.%u", &class, &x1, &x2, &x3 ) != EOF ){

                    if ( (class > 1 << 8) || (x1 > 1 << 8) || (x2 > 1 << 8) || (x3 > 1 << 8) )
                        kill_them_all( msgid, pida, pidb, pidc );

                    if ( class < 1 << 7 )// class A
                        msg.mtype = 1;
                    else if ( class < 3 << 6 )// class B
                        msg.mtype = 2;
                    else msg.mtype = 3; // class C

                    msg.mtext[0] = (unsigned char)class;
                    msg.mtext[1] = (unsigned char)x1;
                    msg.mtext[2] = (unsigned char)x2;
                    msg.mtext[3] = (unsigned char)x3;

                    msgsnd( msgid, &msg, 4 * sizeof(unsigned char), 0 );

                    msgrcv( msgid, &msg, 4 * sizeof(unsigned char), 4, 0 );

                }

                kill_them_all( msgid, pida, pidb, pidc );

            }
            if ( pidc == 0 ){//son class C

                while(1){

                    msgrcv( msgid, &msg, 4 * sizeof(unsigned char), 3, 0 );

                    printf( "C " );
                    msg.mtext[0] = msg.mtext[0] << 3;
                    msg.mtext[0] = msg.mtext[0] >> 3;

                    res = 0;
                    tmp = (unsigned int) msg.mtext[0];
                    tmp = tmp << 16;
                    res = res | tmp;

                    tmp = (unsigned int) msg.mtext[1];
                    tmp = tmp << 8;
                    res = res | tmp;

                    tmp = (unsigned int) msg.mtext[2];
                    res = res | tmp;

                    printf( "%x\n", res );
                    fflush( stdout );

                    msg.mtype = 4;
                    msgsnd( msgid, &msg, 4 * sizeof(unsigned char), 0 );

                }

            }
            if ( pidb < 0 ) kill_them_all( msgid, pida, pidb, pidc );

        }
        if ( pidb == 0 ){//son class B

            while(1){

                msgrcv( msgid, &msg, 4 * sizeof(unsigned char), 2, 0 );

                printf( "B " );
                msg.mtext[0] = msg.mtext[0] << 2;
                msg.mtext[0] = msg.mtext[0] >> 2;

                res = 0;
                tmp = (unsigned int) msg.mtext[0];
                tmp = tmp << 8;
                res = res | tmp;

                tmp = (unsigned int) msg.mtext[1];
                res = res | tmp;

                printf( "%x\n", res );
                fflush( stdout );

                msg.mtype = 4;
                msgsnd( msgid, &msg, 4 * sizeof(unsigned char), 0 );

            }

        }
        if ( pidb < 0 ) kill_them_all( msgid, pida, pidb, pidc );

    }
    if ( pida == 0 ){//son class A

        while(1){

            msgrcv( msgid, &msg, 4 * sizeof(unsigned char), 1, 0 );

            printf( "A " );
            msg.mtext[0] = msg.mtext[0] << 1;
            msg.mtext[0] = msg.mtext[0] >> 1;

            res = (unsigned int) msg.mtext[0];

            printf( "%x\n", res );
            fflush( stdout );

            msg.mtype = 4;
            msgsnd( msgid, &msg, 4 * sizeof(unsigned char), 0 );

        }

    }
    if ( pida < 0 ) kill_them_all( msgid, pida, pidb, pidc );

}