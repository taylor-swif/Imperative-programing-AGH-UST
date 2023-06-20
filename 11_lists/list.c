#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement *next;
    void *data;
} ListElement;

typedef struct {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    printf("malloc error\n");
    exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
    void *ptr = strdup(string);
    if(ptr) return ptr;
    printf("strdup error\n");
    exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, DataFp modify_data) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
    ListElement *element = p_list->head;
    while (element != NULL){
        p_list->dump_data(element->data);
//        printf("%d ", *(int *) (element->data));
        element = element->next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
}

// Free all elements of the list
void free_list(List* p_list) {
    p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = data;
    if (p_list->head == NULL){ // if this element is first
        p_list->tail = element;
        element->next = NULL;
    }
    else{
        element->next = p_list->head;
    }
    p_list->head = element;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement *element = safe_malloc(sizeof(ListElement));
    element->data = data;
    if (p_list->head == NULL){ // if this element is first
        p_list->tail = element;
        p_list->head = element;
        element->next = NULL;
    }
    else{
        p_list->tail->next = element;
        element->next = NULL;
        p_list->tail = element;
    }
}

// Remove the first element
void pop_front(List *p_list) {
    ListElement *next = p_list->head->next;
    p_list->free_data(p_list->head->data);
//    p_list->head = p_list->head->next;
    free(p_list->head);
    p_list->head = next;
    if (next == NULL){
        p_list->tail = NULL;
    }
}

// Reverse the list
void reverse(List *p_list) {
    ListElement* ptr = p_list->head;
    ListElement* shadow = NULL;
    ListElement* temp = NULL;
    temp = p_list->tail;
    p_list->tail = p_list->head;
    p_list->head = temp;
    while (ptr != NULL) {
        temp = ptr->next;
        ptr->next = shadow;
        shadow = ptr;
        ptr = temp;
    }
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement *element = safe_malloc(sizeof (ListElement));
    element->data = data;
    element->next = previous->next;
    previous->next = element;
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    ListElement *element = p_list->head;
    if (element == NULL || p_list->compare_data(element->data, p_data) > 0){
        push_front(p_list, p_data);
    }
    // skok?
    while (element->next != NULL && p_list->compare_data(element->data, p_data) < 0){
        element = element->next;
    }
    if (element->next == NULL){
        push_back(p_list, p_data);
    }
    else if  (p_list->compare_data(element->data, p_data) > 0){
     return;
    }
    else{
        push_after(p_list, p_data, element);
    }
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
    printf("%d ", *(int*)d);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int *create_data_int(int v) {
    int *data = safe_malloc(sizeof (int));
    *data = v;
    return data;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    printf("%s ", ((DataWord *)d)->word);
}

void to_lower(char *s){
    for (char*p = s; *p; p++){
        *p = tolower(*p);
    }
}

void dump_word_lowercase(const void *d){
    to_lower(((DataWord *) d)->word);
    printf("%s ", ((DataWord *) d)->word);
}

void free_word(void *d) {
    DataWord *ptr = (DataWord *)d;
    free(ptr->word);
    free(ptr);
}

int cmp_word_alphabet(const void *a, const void *b) {
    return strcmp(((DataWord *)a)->word, ((DataWord *)b)->word);
}

int cmp_word_counter(const void *a, const void *b) {
    return ((DataWord *)a)->counter - ((DataWord *)b)->counter;
}

void modify_word(void *p) {
    ((DataWord *)p)->counter++;
}

void *create_data_word(const char *string, int counter) {
    DataWord *word = safe_malloc(sizeof (DataWord));
    word->word = safe_strdup(string);
    word->counter = counter;
    return word;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    char line[BUFFER_SIZE];
    const char sep1[]=" \n\t.,:;-!?";
    char *ptr;
    DataWord *word;
    size_t len = 0;
    while (fgets(line, BUFFER_SIZE, stdin)){
        for (ptr = strtok(line, sep1); ptr; ptr = strtok(NULL, sep1)){
            size_t len = strlen(ptr);
            printf("%s %d \n", ptr, len);
            word = create_data_word(ptr, 1);
            if (cmp){
                to_lower(ptr);
                insert_in_order(p_list, ptr);
            }
            push_back(p_list, word);
        }

    }
}

// test integer list
void list_test(List *p_list, int n) {
    char op;
    int v;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        switch (op) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d",&n);
            init_list(&list, dump_int, free_int, cmp_int, NULL);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word, NULL, NULL);
            stream_to_list(&list, stdin, NULL);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d",&n);
            init_list(&list, dump_word_lowercase, free_word, NULL, modify_word);
            stream_to_list(&list, stdin, cmp_word_alphabet);
            list.compare_data = cmp_word_counter;
            DataWord data = { NULL, n };
            dump_list_if(&list, &data);
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

