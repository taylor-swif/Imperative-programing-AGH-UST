#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };
#define DECK_SIZE 52
// Returns an integer from [a,b] using library function rand() and operator %
// if a > b return INT_MIN
// if b - a > RAND_MAX return INT_MAX
// if a == b return a
// else return integer from [a,b]
int rand_from_interval(int a, int b) {
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

int stack_push(int array[], int in, int x) {

    if (in_a < DECK_SIZE){
        array[in] = x;
        top++;
        return OK;
    }
    else{
        return OVERFLOW;
    }
}


int queue_push(int in_nr) { // in_nr clients try to enter the queue
    int in_nr_copy = in_nr;
    if (in < QUEUE_SIZE){
        while(in < QUEUE_SIZE && in_nr > 0){
            queue[in] = curr_nr++ + 1;
            in_nr--;
            in++;
        }
        if (in_nr > 0){
            curr_nr += in_nr;
            return OVERFLOW;
        }
    }
    else{
        curr_nr += in_nr;
    }
}

int queue_pop(int out_nr) {
    if (out_nr > in){
        in = 0;
        for(int i = 0; i < out_nr; i++){
            for(int i = 0; i < QUEUE_SIZE-1; i++){
                queue[i] = queue[i+1];
            }
        }

        return UNDERFLOW;
    }

    for(int i = 0; i < out_nr; i++){
        for(int i = 0; i < QUEUE_SIZE-1; i++){
            queue[i] = queue[i+1];
        }
    }
    in -= out_nr;
    return in;

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
    int in_a = DECK_SIZE/2;
    int in_b = DECK_SIZE/2;
    int table[DECK_SIZE];


    while (n > 0 && in_a > 0 && in_b > 0){
        int res = compare(A[0], B[0]);
        if (res == 1){
            push(B, A[0]);
            pop(A)
        }
        else if (res == -1){
            push(A, B[0]);
            pop(B);
        }
        else{

        }

    }


}


int main(void) {
    int to_do = 1, seed;
    int n;

    scanf("%d %d", &seed, &to_do, &n);
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