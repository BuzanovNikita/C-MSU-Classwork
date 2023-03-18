#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define N 50
#define SIZEBUF 17
#define SIZEERROR 5


int main( int argc, char** argv ){
    char buf[SIZEBUF + 1];
    char *nl = "\n";
    char *er = "Error";
    char *p_nl, *pred;
    int newPiece, oldPiece=0;
    int fdr, fdw, tmp, flag = 1;

    if (argc != 3){
	write( 2, er, SIZEERROR );
        return 0;
    }

    fdr = open( argv[1], O_RDONLY, 0664 );
    fdw = creat( argv[2], 0644 );

    if ( (fdr==-1) || (fdw==-1) ){
	    write( 2, er, SIZEERROR );
        if (fdr!=-1) close(fdr);
        if (fdw!=-1) close(fdw);
        return 0;
    }

    tmp = read(fdr, buf, SIZEBUF);
    while(tmp){

        pred = buf;
        p_nl = strstr( buf, nl );

        if (p_nl) {
            newPiece = p_nl - buf;

            if (flag){

                if (oldPiece + newPiece <= N) {
                    write(fdw, buf, newPiece + 1);
                } else {
                    buf[N - oldPiece] = '\n';
                    write(fdw, buf, N - oldPiece + 1);
                }

            }

            flag = 1;
            oldPiece = 0;
            pred = p_nl + 1;
            p_nl = strstr(pred, nl);

            while (p_nl) {

                newPiece = p_nl - pred;

                if (newPiece <= N) {
                    write(fdw, pred, newPiece + 1);
                } else {
                    pred[N] = '\n';
                    write(fdw, pred, N + 1);
                }

                pred = p_nl + 1;
                p_nl = strstr(pred, nl);

            }

        }

        newPiece = tmp - ( pred - buf );

        if ( flag ){
            if ( newPiece + oldPiece >= N ){
                write( fdw, pred, N - oldPiece );
                write( fdw, nl, 1 );
                flag = 0;
                oldPiece = 0;
            }
            else{
                write( fdw, pred, newPiece );
                oldPiece = oldPiece + newPiece;
            }

        }

        tmp = read( fdr, buf, SIZEBUF );

    }


    close ( fdr );
    close ( fdw );
    return 0;
}
