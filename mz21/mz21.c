#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/*void readf( int fdr, int n ){
    int num;
    while (n>0){
        scanf( "%d", &num );
        write( fdr, &num, sizeof(int) );
        n--;
    }
    lseek( fdr, 0, SEEK_SET );
}
void writef( int fd ){
    int tmp, num;
    tmp = read( fd, &num, sizeof(int) );
    while (tmp){
        printf( "%d\n", num );
        tmp = read( fd, &num, sizeof(int) );
    }
    lseek( fd, 0, SEEK_SET );
}*/

void shiftfile( int fd, int numw ){
    int numr, tmp;
    tmp = read( fd, &numr, sizeof(int) );
    while (tmp){
        lseek( fd, -sizeof(int), SEEK_CUR );
        write( fd, &numw, sizeof(int) );
        numw = numr;
        tmp = read( fd, &numr, sizeof(int) );
    }
    write( fd, &numw, sizeof(int) );
    lseek( fd, 0, SEEK_SET );
}

void addel( int fd, int setnum ){
    int tmp, num;

    tmp = read( fd, &num, sizeof(int) );
    while ( (setnum>num) && (tmp) ) tmp = read( fd, &num, sizeof(int) );
    if ( tmp ){
        lseek( fd, -sizeof(int), SEEK_CUR);
        shiftfile( fd, setnum );
    }
    else write( fd, &setnum, sizeof(int) );

    lseek( fd, 0, SEEK_SET );
}

int main( int argc, char** argv ){
	int fdr, fdw;
	int num;
	int tmp;
	if ( (argc>3) || (argc==1) ) return 1;

	fdr = open( argv[1], O_RDONLY );
	fdw = open( argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644 );

	if ( (fdr==-1) || (fdw==-1) ) {
		if ( fdr!=-1 ) close(fdr);
		if ( fdw!=-1 ) close(fdw);
		return 1;
	}

    /*scanf( "%d", &tmp );
    readf( fdr, tmp );*/

    tmp = read( fdr, &num, sizeof(int) );
    while (tmp==sizeof(int)){
        addel( fdw, num );
        tmp = read( fdr, &num, sizeof(int) );
        lseek( fdw, 0, SEEK_SET );
    }
    if ( (tmp!=sizeof(int)) && (tmp!=0) ){
        close( fdw );
        fdw = open( argv[2], O_RDWR | O_TRUNC );
        close( fdw );
    }


	close( fdr );
	close( fdw );
	return 0;
}
