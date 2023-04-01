#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define MAX_DIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))


#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define MAX_LINE 128

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}
int compare_idx(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;

    // return (arg1 > arg2) - (arg1 < arg2); // possible shortcut
    // return arg1 - arg2; // erroneous shortcut (fails if INT_MIN is present)
}

bool is_char(char c) {
    if (c < LAST_CHAR && c >= FIRST_CHAR) {
        return true;
    } else
        return false;
}

bool is_blank(char c) {
    if (c == ' ' || c == '\t' || c == '\n') {
        return true;
    } else
        return false;
}
// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {

    char line[MAX_LINE];
    int last = 0;
    while (fgets(line, MAX_LINE, stdin)){
        for (int i = 0; i < strlen(line); ++i) {
            if (last == 0 && is_char(line[i])){
                last = 1;
            }
            if (last == 1 && is_blank(line[i])){
                *nw += 1;
                last = 0;
            }
        }
        *nl += 1;
        *nc += strlen(line);
    }

}
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void char_count(int char_no, int *n_char, int *cnt) {
    char line[MAX_LINE];
    int chars[LAST_CHAR - FIRST_CHAR] = {0};
    int indexes[LAST_CHAR - FIRST_CHAR];

    for (int i = 0; i < LAST_CHAR - FIRST_CHAR; ++i){
        indexes[i] = i;
    }

    while (fgets(line, MAX_LINE, stdin)){
        for (int i = 0; i < strlen(line); ++i) {
            if (is_char(line[i])) {
                chars[line[i] - FIRST_CHAR] += 1;
            }
        }
    }
    for (int i = 0; i < LAST_CHAR - FIRST_CHAR; ++i){
        for (int j = 0; j < LAST_CHAR - FIRST_CHAR - 1 - i; ++j){
            if (chars[indexes[j]] > chars[indexes[j+1]]){
                swap(&indexes[j], &indexes[j + 1]);
            }
        }
    }
    *n_char = chars[indexes[LAST_CHAR - FIRST_CHAR - 1 - char_no]];
    *cnt = indexes[LAST_CHAR - FIRST_CHAR - 1 - char_no];
}


void bigram_count(int bigram_no, int bigram[]) {
    char line[MAX_LINE];
    int i;

    int ind[MAX_DIGRAMS];

    for(int i = 0; i < MAX_DIGRAMS; i++) {
        ind[i] = i;
    }

    while (fgets(line, MAX_LINE, stdin)){
        i = 1;
        while(line[i] != '\0' && line[i] != '\n'){
            if (line[i]< LAST_CHAR && line[i] >= FIRST_CHAR && line[i - 1] < LAST_CHAR && line[i - 1] >= FIRST_CHAR){
                //printf("%c%c \n", line[i - 1], line[i]);
                count[(line[i - 1] - FIRST_CHAR) * MAX_CHARS + line[i] - FIRST_CHAR]++;
            }
            i += 1;

        }
    }

    qsort(ind, MAX_DIGRAMS, sizeof(int), cmp_di);
    i = ind[bigram_no];
    bigram[0] = FIRST_CHAR + (i / MAX_CHARS);
    bigram[1] = FIRST_CHAR + (i % MAX_CHARS);
    bigram[2] = count[i];

}


void find_comments(int *line_comment_counter, int *block_comment_counter) {
}

// void digram_count(int digram_no, int digram[], FILE *stream){
//     char last = fgetc(digram_no), next;
//     int ind[MAX_DIGRAMS];

//     for(int i = 0; i < MAX_DIGRAMS; i++) {
//         ind[i] = i;
//     }

//     while(!feof(stream)){
//         next = fgetc(stream);
//         count[(last - FIRST_CHAR) * MAX_CHARS + next - FIRST_CHAR]++;
//         last = next;
//     }

//     qsort(ind, MAX_DIGRAMS, sizeof(int), cmp_di);

//     int i = ind[digram_no];
//     digram[0] = FIRST_CHAR + (i / MAX_CHARS);
//     digram[1] = FIRST_CHAR + (i % MAX_CHARS);
//     digram[2] = count[i];

// }


int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl = 0, nw = 0, nc = 0, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];
    //int digram[3];



    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            //digram_count(char_no, digram, stdin);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            //printf("%c%c %d\n", digram[0], digram[1], digram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

