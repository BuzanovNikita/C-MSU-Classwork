#include <stdio.h>
#define SHIFTYEAR >>9
#define DELETEYEAR <<7
#define DELETEDAY >>12
#define DELETEYEARMON <<11
#define SHIFTDAY >>11
#define LESSTEN 10
#define CENTURE 49
int main(){
    unsigned short in, copy1, copy2;
    unsigned int y, m, d;
    scanf("%hu", &in);

    copy1=in;
    copy2=in;//СОЗДАЁМ ДВЕ КОПИИ, В КОТОРЫХ ОСТАВИМ ТОЛЬКО МЕСЯЦ И ДЕНЬ СООТВЕТСТВЕННО

    in=in SHIFTYEAR;
    y=in;   //ДЕЛАЕМ СДВИГ ЧТОБЫ ОСТАВИТЬ ТОЛЬКО ГОД И СТЕРЕТЬ ДЕНЬ И МЕСЯЦ

    copy1=copy1 DELETEYEAR;
    copy1=copy1 DELETEDAY;
    m=copy1;

    copy2=copy2 DELETEYEARMON;
    copy2=copy2 SHIFTDAY;
    d=copy2;
    if (y>CENTURE) {printf("19");}
    else {printf("20");}
    if (y<LESSTEN) { printf("0");}
    printf("%d %d %d\n", y, m, d);
    return 0;
}