#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define MAX_LINE 500

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];
char maine[][MAX_IDS];


const char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
};

bool is_start_char(char c){
    if ((c >= 'A' && c <= 'Z') || c == '_' || (c >= 'a' && c <= 'z')){
        return true;
    }
    else{
        return false;
    }
}

bool is_legal_char(char c){
    if ((c >= 'A' && c <= 'Z') || c == '_' || (c >= 'a' && c <= 'z') || (c >= 48 && c <= 57)){
        return true;
    }
    else{
        return false;
    }
}

bool is_blank(char c) {
    if (c == ' ' || c == '\t' || c == '\n') {
        return true;
    } else
        return false;
}

int find_idents() {

    char line[MAX_LINE];
    int ids = 0;

    bool in_comment = false;
    bool in_id = false;
    bool in_quote = false;
    int start, stop;
    int k = -1;
    while (fgets(line, MAX_LINE, stdin)){
        printf("%s", line);
        k += 1;
        printf("\nk:%d\n",k);
        for (int i = 0; i < strlen(line); i++){
            if (i < strlen(line) - 1){
                if (in_comment && line[i] == '*' && line[i + 1] == '/') {
                    in_comment = false;
                    i += 1;
                }
                if (!in_comment && line[i] == '/' && line[i + 1] == '*') {
                    in_comment = true;
                    i += 1;
                }
                if (!in_quote && !in_comment && line[i] == '/' && line[i + 1] == '/') {
                    break;
                }
            }
            if (!in_comment && in_quote && line[i] == '"'){
                in_quote = false;
                i += 1;
            }
            if (!in_comment && !in_quote && line[i] == '"'){
                in_quote = true;
                i += 1;
            }

            if (!in_comment && !in_quote){
                if (!in_id && is_start_char(line[i])){
                    in_id = true;
                    start = i;
                    continue;
                }
                if (in_id) {
                    if (is_blank(line[i]) || !is_legal_char(line[i])) {
                        in_id = false;
                        stop = i - 1;
                        printf("%d %d %d ", k, start, stop); // debug
                        // sprawdzanie czy juz nalezy
                        char slice[MAX_ID_LEN];
                        strncpy(slice, line + start, stop - start + 1);
                        slice[stop - start + 1] = '\0';
                        printf("%s", slice); // debug
                        printf("\n"); // debug

                        bool new = true;
                        for (int j = 0; j < 32; j++){
                            if (strcmp(keywords[j], slice) == 0){
                                new = false;
                                printf("pomidor\n");
                                break;
                            }
                        }
                        if (new){
                            for (int j = 0; j < ids; j++){
                                if (strcmp(maine[j], slice) == 0){
                                    new = false;
                                    printf("pomidor %s\n", maine[j]);
                                    break;
                                }
                            }
                        }
                        memset(slice, 0, MAX_ID_LEN);
                        if (new){
                            strcpy(&maine[ids], &slice);
                            ids += 1;
                        }
                        printf("\nwypisaywanie\n");
                        for (int j = 0; j < ids; j++){
                            printf("%s\n", maine[j]);
                        }

                    }
//                    else if (!is_legal_char(line[i])) {
//                        in_id = false;
//                    }
                }

            }
            printf("\nstate: %d %d %d %d %d\n", in_quote, in_comment, in_id, k, i);
        }
        in_id = false;
        printf("\nstate: %d %d %d %d %d\n", in_quote, in_comment, in_id);
    }
    return ids;
}

int cmp(const void* first_arg, const void* second_arg) {
    char *a = *(char**)first_arg;
    char *b = *(char**)second_arg;
    return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
    int a = *(int*)first_arg;
    int b = *(int*)second_arg;
    return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
//printf("%d", strcmp(keywords[0], "auto"));
    printf("%d\n", find_idents());
    return 0;
}

