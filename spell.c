#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include "dictionary.h"


bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
    //prevent weird pointers
    memset(hashtable, 0x00, HASH_SIZE * sizeof(node*));

    //make sure the file opens properly
    FILE* fp = fopen(dictionary_file, "r");
    if (fp == NULL) return false;

    size_t n = 0;
    char* line = NULL;
    ssize_t read = 0;
    int index = 0;
    struct node* curnode;
    struct node* targetNode;

    while((read = getline(&line, &n, fp)) != -1){
        //generate and clear a new node
        curnode = (node*) malloc(sizeof(node));
        memset(curnode, 0x00, sizeof(node));
        strncpy(curnode->word, line, LENGTH); //copy in all but the last char (the newline)
        curnode->word[LENGTH] = '\x00'; //null terminate it just in case
        for (int i=strlen(curnode->word);i>0; i--){
            if (curnode->word[i] == '\n'){
                curnode->word[i] = '\x00';
                break;
            }
        }
        //clean up mem used by getline
        free(line);
        line = NULL;

        index = hash_function(curnode->word);
        targetNode = hashtable[index];
        if (targetNode == NULL) {
            //first in the bucket
            hashtable[index] = curnode; 
        }
        else {
            //not first, walk down the bucket and put it at the end
            while(targetNode->next!=NULL){
                targetNode = targetNode->next;
            }
            targetNode->next=curnode;
        }

    }
    return true;
}

bool word_in_table(const char* word, hashmap_t hashtable[]){
    int res = 0;
    node* n = hashtable[hash_function(word)];
    while(n != NULL){
        res = strncasecmp(n->word, word, strlen(word));
        if (res == 0) return true;
        else n = n->next;
    }
    return false;
}

bool check_word(const char* word, hashmap_t hashtable[]){
    if (word_in_table(word, hashtable)) return true;
    bool isNumeric = true;
    for (int i=0; i<strlen(word); i++){
        if (isalpha(word[i]) || word[i] < 0) isNumeric = false;
    }
    if (isNumeric) return true;

    char* lowered = (char*) malloc(sizeof(char)*strlen(word)+1);
    memset(lowered, 0x00, strlen(word)+1);
    strncpy(lowered, word, strlen(word));
    lowered[0] = tolower(lowered[0]);
    bool value = false;
    if (word_in_table(lowered, hashtable)) value = true;
    free(lowered);
    lowered = NULL;
    return value;

}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]){

    if (fp == NULL) return -1;
    int wrong = 0;
    memset(misspelled, 0x00, MAX_MISSPELLED * sizeof(char*));

    char* curWord = (char*) malloc(sizeof(char) * LENGTH);
    memset(curWord, 0x00, LENGTH);

    size_t n = 0;
    char* line = NULL;
    ssize_t read = 0;
    int i = 0;
    int index_in_line = 0;

    while((read = getline(&line, &n, fp)) != -1){
        index_in_line = 0;
        while (index_in_line < read){
            while (index_in_line < read && !isalnum(line[index_in_line])) index_in_line++;
            while (index_in_line < read && line[index_in_line] != ' ' && i < LENGTH){
                curWord[i] = line[index_in_line];
                i++;
                index_in_line++;
            }
            if (i == LENGTH) curWord[i] = '\x00';
            i = strlen(curWord);            
            while(!isalnum(curWord[i]) && i>=0){
                curWord[i--] = '\x00';
            }

            if (strlen(curWord) > 0 && !check_word(curWord, hashtable)){
                misspelled[wrong++] = curWord;
            }
            else {
                free(curWord);
            }
            curWord = (char*) malloc(sizeof(char) * LENGTH);
            memset(curWord, 0x00, LENGTH);
            i = 0;
            index_in_line++;
        }
    }
    return wrong;
}