#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 21

typedef struct s_list {
    char* word;

    struct s_list *next;
} t_list;


char* read_word(){
    char c;
    char *word=NULL;
    int i=0;

    c = getchar();
    while ( (c=='\n') || (c==' ') ){
        c = getchar();
    }

    if ( c != EOF ){
        word = (char*)malloc( sizeof( char ) );
        if (!word) return word;
        while ( (c!='\n') && (c!=' ') && (c!=EOF) ){
            word[i] = c;
            i++;
            c = getchar();
        }
        word[i] = '\0';
    }

    return word;
}

t_list *create_list(){
    char* set_word;
    t_list* head=NULL;
    t_list* node;

    set_word = read_word();
    if ( set_word != NULL ){
        head=(t_list*)malloc( sizeof( t_list ) );
        if (!head) return head;
        node = head;
        node -> word = set_word;

        set_word = read_word();
        while ( set_word != NULL ){
            node -> next = (t_list*)malloc( sizeof( t_list ) );
            if (!(node->next)) break;
            node = node -> next;
            node -> word = set_word;
            set_word = read_word();
        }
        node -> next = NULL;
    }

    return head;

}

void delete_trash ( t_list** head, char* trash ){
    t_list* node=*head;
    t_list* tmp;
    if ( node == NULL ) return;
    if (!strcmp(node->word,trash)){
        tmp = *head;
        *head = (*head) -> next;
        free( tmp -> word );
        free( tmp );
        return;
    }
    if ( node -> next != NULL ){
        while ( node -> next -> next != NULL ){
            if ( !strcmp(node->next->word, trash) ){
                tmp = node -> next;
                node -> next = node -> next -> next;
                free( tmp -> word );
                free( tmp );
                return;
            }
            node = node -> next;
        }
        if (!strcmp(node->next->word, trash)){
            free( node -> next -> word );
            free( node -> next);
            node->next = NULL;
        }
    }
}

void print_and_free( t_list** head ){
    t_list *node=*head;
    t_list *tmp;

    if ( node == NULL ){
        printf("empty");
    }
    else{
            tmp = node;
            printf( "%s", node -> word );
            node = node -> next;

            free( tmp -> word );
            free( tmp );

        while ( node != NULL ){
            tmp = node;
            printf( " %s", node -> word );
            node = node -> next;

            free( tmp -> word );
            free( tmp );
        }
    }
}



int main(){
    char* trash=read_word();
    t_list *head;
    if ( trash == NULL ) {
        head = NULL;
    }
    else {
        head = create_list();
    }
    delete_trash( &head, trash );
    free( trash );
    print_and_free( &head );
}
