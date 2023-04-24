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
void print_tab(int A[], int n){
    printf("---\n");
    for (int i = 0; i < n; i++){
        printf("%d ", A[i]);
    }
    printf("\n");

}

void char_count(int char_no, int *n_char, int *cnt) {
    char line[MAX_LINE];
    int indices[MAX_CHARS];

    for (int i = 0; i < MAX_CHARS; ++i){
        indices[i] = i;
    }

    while (fgets(line, MAX_LINE, stdin)){
        for (int i = 0; i < strlen(line); ++i) {
            if (is_char(line[i])) {
                count[line[i] - FIRST_CHAR] += 1;
            }
        }
    }

    qsort(indices, MAX_CHARS, sizeof (int), cmp);

    *n_char = FIRST_CHAR + indices[char_no - 1];
    *cnt = count[indices[char_no - 1]];
}


void bigram_count(int bigram_no, int bigram[]) {

    char line[MAX_LINE];
    int indices[MAX_BIGRAMS];

    for(int i = 0; i < MAX_BIGRAMS; i++) {
        indices[i] = i;
    }

    while (fgets(line, MAX_LINE, stdin)){

        for (int i = 0; i < strlen(line) - 1; i++) {
            if (is_char(line[i]) && is_char(line[i + 1])) {
                count[(line[i] - FIRST_CHAR) * MAX_CHARS + line[i + 1] - FIRST_CHAR]++;
            }
        }
    }

    qsort(indices, MAX_BIGRAMS, sizeof(int), cmp_di);
    int i = indices[bigram_no - 1];
    bigram[0] = FIRST_CHAR + (i / MAX_CHARS);
    bigram[1] = FIRST_CHAR + (i % MAX_CHARS);
    bigram[2] = count[i];

}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    char line[MAX_LINE];
    bool is_block = false;

    while (fgets(line, MAX_LINE, stdin)){
        for (int i = 0; i < strlen(line) - 1; ++i) {
            if (line[i] == '\0') break;

            if (line[i] == '*' && line[i + 1] =='/' && is_block){
                is_block = false;
                *block_comment_counter += 1;
                i++;
            }
            if (line[i] == '/' && line[i + 1] =='/' && !is_block){
                *line_comment_counter += 1;
                break;
            }
            if (line[i] == '/' && line[i + 1] =='*' && !is_block){
                is_block = true;
                i++;
            }

        }
    }
}

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl = 0, nw = 0, nc = 0, char_no, n_char, cnt;
    int line_comment_counter = 0, block_comment_counter = 0;
    int bigram[3];


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
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
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