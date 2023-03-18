#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int i,j;

int findpalka( char** argv, int argc ){
    int i;
    for( i=1; i<argc; i++ ){
        if ( strcmp( argv[i], "|" ) == 0 ){
            break;
        }
    }
    return i;
}

int main( int argc, char** argv ) {
    int channelfs[2], channelss[2], tmp;

    tmp = pipe( channelss );
    if ( tmp == -1 ) return 1;

    tmp = pipe( channelfs );
    if ( tmp == -1 ) {
        close( channelss[0] );
        close( channelss[1] );
        return 1;
    }

    i = findpalka( argv, argc );
    argv[i] = NULL;
    j = findpalka( argv + i , argc - i ) + i;
    argv[j] = NULL;

    tmp = fork();
    if ( tmp == 0 ){//son1
        close( channelfs[1] );
        close( channelfs[0] );
        dup2( channelss[1], 1 );
        close( channelss[1] );
        close( channelss[0] );
        execvp( argv[1],  argv + 1 );
        fprintf( stderr, "exec_error\n" );
        return 1;
    }
    if ( tmp > 0 ){
        tmp = fork();
        if ( tmp == 0 ){//son2
            dup2( channelss[0], 0 );
            close( channelss[0] );
            close( channelss[1] );
            dup2( channelfs[1], 1 );
            close( channelfs[0] );
            close( channelfs[1] );
            execvp( argv[i+1],  argv + i + 1 );
            fprintf( stderr, "exec_error\n" );
            return 1;
        }
        if ( tmp > 0 ){
            tmp = fork();
            if ( tmp == 0 ){//son3
                close( channelss[1] );
                close( channelss[0] );
                dup2( channelfs[0], 0 );
                close( channelfs[0] );
                close( channelfs[1] );
                execvp( argv[j+1],  argv + j + 1 );
                fprintf( stderr, "exec_error\n" );
                return 1;
            }
            if ( tmp > 0 ){//father
                close( channelss[1] );
                close( channelss[0] );
                close( channelfs[0] );
                close( channelfs[1] );
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
            if ( tmp == -1 ){
                fprintf( stderr, "fork_error\n" );
                close( channelss[0] );
                close( channelss[1] );
                close( channelfs[0] );
                close( channelfs[1] );
                return 1;
            }

        }
        if ( tmp == -1 ){
            fprintf( stderr, "fork_error\n" );
            close( channelss[0] );
            close( channelss[1] );
            close( channelfs[0] );
            close( channelfs[1] );
            return 1;
        }
    }
    if ( tmp == -1 ){
        fprintf( stderr, "fork_error\n" );
        close( channelss[0] );
        close( channelss[1] );
        close( channelfs[0] );
        close( channelfs[1] );
        return 1;
    }

    return 0;
}
