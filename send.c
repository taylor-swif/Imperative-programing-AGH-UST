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
int A[DECK_SIZE], B[DECK_SIZE]; 
int deck[DECK_SIZE];
int len_a = 26;
int len_b = 26;

#define QUEUE_SIZE DECK_SIZE

void printf_tab(void){
    for (int i = 0; i < len_a; i++){
        printf("%d ", A[i]);
    }
    printf("\n");
    for (int i = 0; i < len_b; i++){
        printf("%d ", B[i]);
    }
    printf("\n");
    printf("----------------------\n");
}

void printf_tab1(int arr[], int len){

    for (int i = 0; i < len; i++){
        printf("%d ", arr[i]);
    }
    // printf("\n");
    // printf("----------------------\n");
}
int deck_push(int deck [], int card_num, int len) { // card_num enter the queue
    
    if (len < DECK_SIZE){
        deck[len] = card_num;
        return OK;
    }
    else{
        return OVERFLOW;
    }
}

void deck_pop(int array[], int len) {
    if (len ==0 ){
        //return UNDERFLOW;
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
    a = a/4;
    b = b/4;
    if (a == b){
        return 0;
    }
    else if(a < b){
        return 1;
    }
    return -1;

}


int warwar(void){
    int minimum;
    if (len_a <= len_b){
        minimum = len_a;
    }else{
        minimum = len_b;
    }
    int won = -1;
    int cnt = 0;
    //printf("wojna start\n");
    for (int i = 2; i <minimum; i += 2){
        won = compare(A[i], B[i]);
        //printf("wojna i: %d\n", i);
        //printf_tab();
        //printf("%d ", len_a);
        //printf("%d \n", len_b);
        if ( won == 1){
            for (int j = 0; j < i+1; j++){
            deck_push(B, B[0], len_b);
            len_b++;
            deck_pop(B, len_b);
            len_b--;
            }
            for (int j = 0; j < i+1; j++){
            deck_push(B, A[0], len_b);
            deck_pop(A, len_a);
            len_b++;
            len_a--;
            }
            break;
        }else if(won == -1){
            for (int j = 0; j < i+1; j++){
            deck_push(A, A[0], len_a);
            len_a++;
            deck_pop(A, len_a);
            len_a--;
            }
            for (int j = 0; j < i+1; j++){
            deck_push(A, B[0], len_a);
            deck_pop(B, len_b);
            len_b--;
            len_a++;
            }
            break;

        }
        cnt++;
    }
    //printf("doszlo do wojny\n");
    if (won == 0){
        return -1;
        //printf("taki przypadek");
    }
    return cnt;
}

void war0(int n){

    int n_copy = n;

    while (n > 0 && len_a > 0 && len_b > 0){
        int res = compare(A[0], B[0]);
        // printf("flaga\n");
        // printf_tab();
        if (res == 1){
            deck_push(B, B[0], len_b);
            len_b++;
            deck_pop(B, len_b);
            len_b--;
            deck_push(B, A[0], len_b);
            deck_pop(A, len_a);
            len_b++;
            len_a--;
            
        }
        else if (res == -1){
            deck_push(A, A[0], len_a);
            len_a++;
            deck_pop(A, len_a);
            len_a--;
            deck_push(A, B[0], len_a);
            deck_pop(B, len_b);
            len_b--;
            len_a++;
        }
        else{
            int c = warwar();
            if (c < 0){
                printf("yyy\n");
                printf("%d ", c);
                printf("%d \n", len_a);
                printf("%d \n", len_b);
                return;
                
            }else{
                n -= c;
            }
            
        }
        // printf("------------\n");
        // printf_tab(A, len_a);
        // printf_tab(B, len_b);
        n--;

    }
    //printf("koniec\n");
    if (n == 0){
        printf("1 ");
        printf("%d ", len_a);
        printf("%d ", len_b);
        /*printf("------------\n");
        printf_tab(A, len_a);
        printf_tab(B, len_b);*/

    }
    else if (len_a == DECK_SIZE){
        printf("2 ");
        printf("%d ", n_copy-n+1);
    }
    else if (len_b == DECK_SIZE){
        printf("3\n");
        printf_tab1(B, len_b);
    }
    else{
        
        printf("else\n");
        printf("%d \n", n_copy-n);
        printf("%d ", len_a);
        printf("%d ", len_b);
      //  printf_tab(A, len_a);
      //  printf_tab(B, len_b);
        
    }
    //printf_tab();
            
   // printf("ilosc zagranych %d \n", n_copy-n);
}
void war1(int n){

    int n_copy = n;


    while (n > 0 && len_a > 0 && len_b > 0){
        int res = compare(A[0], B[0]);
        // printf("flaga\n");
        if (res == 1){
            deck_push(B, B[0], len_b);
            len_b++;
            deck_pop(B, len_b);
            len_b--;
            deck_push(B, A[0], len_b);
            deck_pop(A, len_a);
            len_b++;
            len_a--;
            
        }
        else if (res == -1){
            deck_push(A, A[0], len_a);
            len_a++;
            deck_pop(A, len_a);
            len_a--;
            deck_push(A, B[0], len_a);
            deck_pop(B, len_b);
            len_b--;
            len_a++;
        }
        else{
            deck_push(A, A[0], len_a);
            len_a++;
            deck_pop(A, len_a);
            len_a--;
            deck_push(B, B[0], len_b);
            len_b++;
            deck_pop(B, len_b);
            len_b--;
            
        }

        n--;

    }
    if (n == 0){
        printf("0\n");
        printf("%d ", len_a);
        printf("%d ", len_b);

    }
    else if (len_a == DECK_SIZE){
        printf("2 ");
        printf("%d ", n_copy-n);
    }
    else if (len_b == DECK_SIZE){
        printf("3\n");
        printf_tab1(B, len_b);
    }
    else{
        printf("4\n");
    }
}


int main(void) {
    int to_do, seed;
    int n;


    //printf_tab(A, len_a);
    //printf_tab(B, len_b);

    scanf("%d %d %d", &seed, &to_do, &n);
    srand((unsigned) seed); // set the seed
    rand_permutation(DECK_SIZE, deck);
    divide(A, B, deck);
    //printf_tab();
    switch(to_do) {
        case 0:
            war0(n);
            break;
        case 1:
            war1(n);
            break;
        default:
            printf("NOTHING TO DO!\n");
            break;
    }
    return 0;
}