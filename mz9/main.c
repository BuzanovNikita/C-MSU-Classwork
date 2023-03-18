#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TURNTOBIG -32
#define TURNTOSMALL +32
#define N 1001

void Clean ( char* str, char*argv ){
    int length, flag=0, shift;
    char *illegal, *afterillegal, *tmp;
    length = strlen( argv );
    illegal = strstr ( str, argv );
    while ( illegal!=NULL ){
        shift = length;
        afterillegal = illegal + length;
        if ( str == illegal ){
            if ( *afterillegal == '\n' ){
                flag = 1;
            }
            else {
                if ( *afterillegal == ' ' ){
                    afterillegal++;
                    shift++;
                    flag = 1;
                }
            }
        }
        else {
            tmp = illegal - 1;
            if ( *tmp == ' ' ){
                if ( (*afterillegal == ' ') || ( *afterillegal == '\n' ) ){
                    illegal--;
                    shift++;
                    flag = 1;
                }
            }
        }
        if ( flag ){
            tmp = illegal;
            while (*afterillegal!='\n'){
                *tmp = *afterillegal;
                tmp++;
                afterillegal++;
            }
            *tmp = '\n';
            tmp++;
            *tmp = '\0';
            flag = 0;
        }
        else {
            illegal = illegal + 1;
        }
        illegal = strstr( illegal, argv );
    }

}

int main ( int argc, char **argv ){
    char *str, *tmp;
    int i;
    while (1) {
        str = malloc ( sizeof( char ) * N );
        if ( str == NULL ){
            return 1;
            break;
        }
        else {
            str = fgets( str, N, stdin);
            if ( str == NULL ){
                break;
            }
            else {
                for ( i=1; i<argc; i++){
                    Clean (str, argv[i]);
                    tmp = argv[i];
                    *tmp = *tmp TURNTOBIG;
                    Clean (str, argv[i]);
                    tmp++;
                    while ( *tmp!='\0' ){
                        *tmp = *tmp TURNTOBIG;
                        tmp++;
                    }
                    Clean (str, argv[i]);
                    tmp = argv[i];
                    while ( *tmp != '\0' ){
                        *tmp = *tmp TURNTOSMALL;
                        tmp++;
                    }
                }
                printf("%s", str);
            }
            free (str);
        }
    }
    return 0;

}