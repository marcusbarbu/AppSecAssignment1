#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include "dictionary.h"


bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
    memset(hashtable, 0x00, HASH_SIZE * sizeof(node*));

    FILE* fp = fopen(dictionary_file, "r");
    if (fp == NULL) return false;

    size_t n = 0;
    char* line = NULL;
    ssize_t read = 0;
    int index = 0;
    struct node* curnode;
    struct node* targetNode;

    while((read = getline(&line, &n, fp)) != -1){
        curnode = (node*) malloc(sizeof(node));
        memset(curnode, 0x00, sizeof(node));
        strncpy(curnode->word, line, read-1);
        index = hash_function(curnode->word);
        targetNode = hashtable[index];
        if (targetNode == NULL) {
            hashtable[index] = curnode; 
        }
        else {
            while(targetNode->next!=NULL){
                targetNode = targetNode->next;
            }
            targetNode->next=curnode;
        }
        free(line);
        line = NULL;
    }
    return true;
}

bool check_word(const char* word, hashmap_t hashtable[]){
    //printf("testing word: %sx",word);
    int res;
    node* n = hashtable[hash_function(word)];
    while(n != NULL){
        res = strncasecmp(n->word, word, strlen(word));
        if (res == 0) return true;
        else n = n->next;
    }
    return false;
}

char* cleanWord(char* orig, bool trim){
    char* new_string;
    if (trim){
        new_string = (char*) malloc(sizeof(char)*(strlen(orig)));
        memset(new_string, 0x00, strlen(orig));
        strncpy(new_string, orig, strlen(orig)-1);
    }
    else{
        new_string = (char*) malloc(sizeof(char)*(strlen(orig))+1);
        memset(new_string, 0x00, strlen(orig)+1);
        strncpy(new_string, orig, strlen(orig));
    }

    int removable = 0;
    for (int i=0; i<strlen(new_string); i++){
        new_string[i] = tolower(new_string[i]);
        if ((new_string[i]>0x7A || new_string[i]<0x61) && i==strlen(new_string)-1){
            removable++;
        }
    }
    if (removable != 0){
        char* ret_string;
        ret_string = (char*) malloc(sizeof(char) * (strlen(new_string)-removable)+1);
        memset(ret_string, 0x00, (strlen(new_string)-removable)+1);
        strncpy(ret_string, new_string, strlen(new_string)-removable);
        free(new_string);
        new_string = NULL;
        return ret_string;
    }
    else {
        return new_string;
    }

}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]){

    if (fp == NULL) return -1;
    int wrong = 0;

    size_t n = 0;
    char* curWord = NULL;
    size_t read;
    char* trimmedWord = NULL;

    memset(misspelled, 0x00, MAX_MISSPELLED * sizeof(char*));

    while((read = getdelim(&curWord, &n, ' ', fp)) != -1){
        trimmedWord = cleanWord(curWord, true);

        if (!check_word(trimmedWord, hashtable)){
            misspelled[wrong] = trimmedWord;
            wrong++;
        }
        else {
            free(trimmedWord);
        }
        trimmedWord = NULL;
        memset(curWord, 0x00, read);
    }
    return wrong;
}