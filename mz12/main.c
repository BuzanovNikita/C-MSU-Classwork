#include <stdio.h>
#include <stdlib.h>
/*enum SIDE {
    LEFT,
    RIGHT
};*/

typedef struct s_tree {
    int key;
    struct s_tree *left, *right;
} t_tree;

t_tree* create_node( int set_key ) {
    t_tree *node = (t_tree *) malloc(sizeof(t_tree));
    if ( node == NULL ) return NULL;


    node->key = set_key;
    node -> left = node -> right = NULL;

    return node;
}
/*
void push( t_tree** tree, int set_key ){
    enum SIDE pred_side;
    t_tree* newNode = create_node( set_key );
    t_tree* tmp = *tree, *pred;
    int meet_new = 1;

    if ( tmp == NULL ) {
        *tree = newNode;
    }
    else {
        while ( tmp != NULL ){
            pred = tmp;
            if ( tmp -> key < set_key ) {
                tmp = tmp -> right;
                pred_side = RIGHT;
                continue;
            }
            if ( tmp -> key > set_key ) {
                tmp = tmp -> left;
                pred_side = LEFT;
                continue;
            }
            if ( tmp -> key == set_key ) {
                meet_new = 0;
                break;
            }
        }
        if (meet_new){
            if ( pred_side == RIGHT ) pred -> right = newNode;
            else pred -> left = newNode;
        }
    }
}*/

void push_rec( t_tree** tree, int set_key ){
    if ( *tree == NULL ) {
        *tree = create_node(set_key);
        if (*tree==NULL) return;
    }
    else if ( (*tree) -> key < set_key ) {
        push_rec(&((*tree)->right), set_key);
    }
    else if ( (*tree) -> key > set_key ) push_rec( &((*tree)->left), set_key );
}

t_tree* create_tree(){
    t_tree* tree=NULL;
    int c;
    int set_key;

    c = scanf( "%d", &set_key );
    while ( c == 1 ){
        push_rec( &tree, set_key );
        c = scanf( "%d", &set_key );
    }

    return tree;
}

void print_mins( t_tree** tree){
    t_tree* node=*tree;
    if ( node -> left != NULL) {
        while (node->left->left != NULL) {
            node = node->left;
        }
        if ( node -> left -> right == NULL ){
            printf("%d %d\n", node->left->key, node->key);
        }
        else {
            printf( "%d", node -> left -> key );
            node = node -> left -> right;
            while ( node -> left != NULL ){
                node = node -> left;
            }
            printf(" %d", node -> key );
        }
    }
    else {
        printf("%d", node->key);
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
        printf(" %d\n", node->key);
    }
}

void free_tree( t_tree** tree ){
    t_tree* tmp;
    if ( ((*tree)->left==NULL) && ((*tree)->right==NULL) ){
        tmp = *tree;
        *tree = NULL;
        free( tmp );
    }
    else{
        if ( (*tree)->left!=NULL ) free_tree( &(*tree)->left );
        if ( (*tree)->right!=NULL ) free_tree( &(*tree)->right );
    }
}

int main(){
    t_tree* tree=create_tree();
    if (tree==NULL) return 1;
    print_mins( &tree );
    free_tree( &tree );
    return 0;
}