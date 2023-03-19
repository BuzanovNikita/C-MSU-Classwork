#include <stdio.h>
#define LEFT 15
#define RIGHT 0
#define FIRSTBIT 1<<15
#define SHIFTRIGHT >>1
#define SHIFTYEAR <<9
#define SHIFTDAY <<5

void PrintBits( unsigned short input ){
    int i;
    unsigned short mask=FIRSTBIT;
    for (i=RIGHT; i<=LEFT; i++){
        if (mask & input) {printf("1");}
        else {printf("0");}
        mask=mask SHIFTRIGHT;
    }
    printf("\n");
}

int main(){
    unsigned int y, m, d;
    unsigned short output=0, tmp;
    scanf("%d %d %d", &y, &m, &d);

    tmp = (unsigned short) y;
    tmp = tmp SHIFTYEAR;
    output = output | tmp;

    tmp = (unsigned short) m;
    tmp = tmp SHIFTDAY;
    output = output | tmp;

    tmp = (unsigned short) d;
    output = output | tmp;

    PrintBits( output );
    printf("%hu\n", output);
    //The end
    return 0;
}

