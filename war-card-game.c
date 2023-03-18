#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };
#define DECK_SIZE 52

int rand_from_interval(int a, int b) {

    // Returns an integer from [a,b] using library function rand() and operator %
    // if a > b return INT_MIN
    // if b - a > RAND_MAX return INT_MAX
    // if a == b return a
    // else return integer from [a,b]
    if (a > b){
        return INT_MIN;
    }
    else if (b - a > RAND_MAX){
        return INT_MAX;
    }
    else if (a == b){
        return a;
    }
    else{
        int num = rand()%(b-a+1) + a;
        return num;
    }

}


void swap (int array[], int i, int k) {
    int t = array[k];
    array[k] = array[i];
    array[i] = t;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
    int k;
    for(int i = 0; i < n; i++) array[i] = i;
    for(int i = 0; i < n-1; i++){
        k = rand_from_interval(i, n-1);
        swap(array, i, k);
    }

}
int A[DECK_SIZE], B[DECK_SIZE], deck[DECK_SIZE];
#define QUEUE_SIZE DECK_SIZE


int deck_push(int deck [], int card_num, int len) { // card_num enter the queue
    
    if (len < DECK_SIZE){
        deck[len] = card_num;
        return OK;
    }
    else{
        return OVERFLOW;
    }
}

int deck_pop(int array[], len) {
    if (len ==0 ){
        return UNDERFLOW;
    }
    else{
        for(int i = 0; i < len-1; i++){
            array[i] = array[i+1];
        }
    }
}

void divide(int A[], int B[], int deck[]){
    for (int i = 0; i < DECK_SIZE/2; i++){
        A[i] = deck[i];
        B[i] = deck[i + DECK_SIZE/2];
    }

}

int compare(int a, int b){
    a = a - a%4;
    b = b - b%4;
    if (a == b){
        return 0;
    }
    else if(a < b){
        return 1;
    }
    return -1;

}

int war0(int n){
    int len_a = 26;
    int len_b = 26;
    //int table[DECK_SIZE];


    while (n > 0 && len_a > 0 && len_b > 0){
        int res = compare(A[0], B[0]);
        if (res == 1){
            deck_push(B, A[0], len_b);
            deck_pop(A, len_a);
            
        }
        else if (res == -1){
            deck_push(A, B[0], len_a);
            deck_pop(B, len_b);
        }
        else{

        }

    }
    if (len_a == 0){
        printf("Wygrał gracz A");
    }
    else if (len_b == 52){
        printf("Wygrał gracz B");
    }
    return 0;


}


int main(void) {
    int to_do = 1, seed;
    int n;

    scanf("%d %d %d ", &seed, &to_do, &n);
    srand((unsigned) seed); // set the seed

    switch(to_do) {
        case 0:
            rand_permutation(DECK_SIZE,deck);
            divide(A, B, deck);
            war0(n);
            break;
        case 1:

            break;
        default:
            printf("NOTHING TO DO!\n");
            break;
    }
    return 0;
}