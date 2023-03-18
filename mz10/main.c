#include <stdio.h>
int main( int argc, char** argv ){
    FILE *fo, *fc;
    int sym=0, str=0, word=0, c, flag=0;
    if ((argc>3)||(argc=0)) return 1;
    fo = fopen (argv[1], "r");
    if ( fo == NULL ){
        return 1;
    }
    c = fgetc(fo);
    while ( c != EOF ){
        if ( c=='\n' ){
            str++;
            if (flag) word++;
            flag = 0;
        }
        else {
            if ( c==' ' ){
                if (flag) word++;
                flag = 0;
            }
            else {flag=1;}
        }
        sym++;
        c = fgetc(fo);
    }
    fclose(fo);
    fc = fopen( argv[2], "w" );
    if ( fc == NULL) return 1;
    fprintf( fc, "%d %d %d", str, word, sym);
    fclose(fc);
    return 0;
}