#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair* relation, int size) {
    int digit;
    for(int i = 0; i < size; i++){
        digit = relation[i].first;
        for (int j = 0; j < size; j++){
            if (i < j && relation[j].first == digit){
                break;
            }
            if (relation[j].first == digit && relation[j].second == digit){
                break;
            }
            if(j == size - 1 && (relation[j].first != digit || relation[j].second != digit)){
                return 0;
            }
        }
        digit = relation[i].second;
        for (int j = 0; j < size; j++){
            if (i < j && relation[j].second == digit){
                break;
            }
            if (relation[j].first == digit && relation[j].second == digit){
                break;
            }
            if(j == size - 1 && (relation[j].first != digit || relation[j].second != digit)){
                return 0;
            }
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair* relation, int size){
    int digit;
    for(int i = 0; i < size; i++){
        digit = relation[i].first;
        for (int j = 0; j < size; j++){
            if (i < j && relation[j].first == digit){
                break;
            }
            if (relation[j].first == digit && relation[j].second == digit){
                return 0;
            }
        }
        digit = relation[i].second;
        for (int j = 0; j < size; j++){
            if (i < j && relation[j].second == digit){
                break;
            }
            if (relation[j].first == digit && relation[j].second == digit){
                return 0;
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair* relation, int size){
    int first, second;
    for(int i = 0; i < size; i++) {
        first = relation[i].first;
        second = relation[i].second;
        for (int j = 0; j < size; j++) {

            if (relation[j].first == second && relation[j].second == first) {
                break;
            }
            if (j == size - 1 && (relation[j].first != second || relation[j].second != first)) {
                return 0;
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair* relation, int size){
    int first, second;
    for(int i = 0; i < size; i++) {
        first = relation[i].first;
        second = relation[i].second;
        for (int j = 0; j < size; j++) {

            if (relation[j].first == second && relation[j].second == first) {
                if(first != second){
                    return 0;
                }
                break;
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair* relation, int size){
    int first, second;
    for(int i = 0; i < size; i++) {
        first = relation[i].first;
        second = relation[i].second;
        for (int j = 0; j < size; j++) {

            if (relation[j].first == second && relation[j].second == first) {
                return 0;
            }
        }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair* relation, int size){
    int first, second, third;
    for(int i = 0; i < size; i++) {
        first = relation[i].first;
        second = relation[i].second;

        for (int j = 0; j < size; j++) {

            if (relation[i].first == second) {
                third = relation[i].second;

                for (int k = 0; k < size; k++){

                    if(relation[k].first == first && relation[k].second == third){
                        break;
                    }
                    if (k == size - 1 && (relation[j].first != first || relation[j].second != third)) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

// Case 2:
int does_contain(int digit, int len, const int* domain){
    for (int i = 0; i < len; i++){
        if (domain[i] == digit){
            return 1;
        }
    }
    return 0;
}

int does_contain_pair(int digit, int digit1, int len, const pair* domain){
    for (int i = 0; i < len; i++){
        if (domain[i].first == digit && domain[i].second == digit1){
            return 1;
        }
    }
    return 0;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int *array, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n - 1 - i; j++){
            if (array[j] > array[j + 1]){
                swap(&array[j],  &array[j + 1]);
            }
        }
    }
}

int get_domain(const pair* relation, int size, int* domain){
    int len = 0;
    for (int i = 0; i < size; i ++){
        if (!does_contain(relation[i].first, len, domain)){
            domain[len] = relation[i].first;
            len++;
        }
        if (!does_contain(relation[i].second, len, domain)){
            domain[len] = relation[i].second;
            len++;
        }
    }
    bubbleSort(domain, len);
    return len;
}

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair* relation, int size){
    if(is_reflexive(relation, size) && is_transitive(relation, size) && is_antisymmetric(relation, size)){
        return 1;
    }
    else{
        return 0;
    }
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair* relation, int size, int x, int y){
    for (int i = 0; i < size; i++){
        if ((relation[i].first == y && relation[i].second == x) || (relation[i].first == x && relation[i].second == y)){
            return 1;
        }
    }
    return 0;
}

// A total order relation is a partial order relation that is connected
int is_total_order(const pair* relation, int size, const int* domain, int len,  int ordered) {
    if (ordered) {
        for (int i = 0; i < len; i++){
            for (int j = i; j < len; j++){ // czy dla takich samych tez?
                if (!is_connected(relation, size, domain[i], domain[j + 1])){
                    return 0;
                }
            }
        }
        return 1;
    }
    else {
        return 0;
    }

};



int find_max_elements(const pair* relation, int size, int* max_elements, const int* domain, int len){
    int cnt = 0;
    for (int j = 0; j < len; j++){
        int bool = 1;
        for (int i = 0; i < size; i++){
            if (relation[i].first == domain[j] && relation[i].second != domain[j]){
                bool = 0;
                break;
            }
        }
        if (bool){
            max_elements[cnt] = domain[j];
            cnt++;
        }
    }
    return cnt;
}
int find_min_elements(const pair* relation, int size, int* min_elements, const int* domain, int len){
    int cnt = 0;
    for (int j = 0; j < len; j++){
        int bool = 1;
        for (int i = 0; i < size; i++){
            if (relation[i].second == domain[j] && relation[i].first != domain[j]){
                bool = 0;
                break;
            }
        }
        if (bool){
            min_elements[cnt] = domain[j];
            cnt++;
        }
    }
    return cnt;
}




// Case 3:

int composition (const pair* relation, int size , const pair* relation_2, int size_2, pair* comp_relation){
    int cnt = 0;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size_2; j++) {
            if (relation[i].second == relation_2[j].first) {
                if(!does_contain_pair(relation[i].first, relation_2[j].second, cnt, comp_relation)){
                    comp_relation[cnt].first = relation[i].first;
                    comp_relation[cnt].second = relation_2[i].second;
                    cnt++;
                }
            }
        }
    }
    return cnt;
}

// Comparator for pair
//int cmp_pair (const void *a, const void *b) {
//}
//
//int insert_int (int *tab, int n, int new_element) {
//}
//
//// Add pair to existing relation if not already there
//int add_relation (pair *tab, int n, pair new_pair) {
//}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int size;
    scanf("%d", &size);
    int first, second;
    for (int i = 0; i < size; i++){
        scanf("%d", &first);
        scanf("%d", &second);
        relation[i].first = first;
        relation[i].second = second;
    }
    return size;
}

void print_int_array(const int *array, int n) {
    for (int i = 0; i < n; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d",&to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size, domain, n_domain, ordered));
            printf("%d\n", n_domain);
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int no_max_elements = find_max_elements(relation, size, max_elements, domain, n_domain);
            int no_min_elements = find_min_elements(relation, size, min_elements, domain, n_domain);
            printf("%d\n", no_max_elements);
            print_int_array(max_elements, no_max_elements);
            printf("%d\n", no_min_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

