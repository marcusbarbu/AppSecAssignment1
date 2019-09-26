#include "dictionary.h"
#include <stdbool.h>

#define DICTIONARY "wordlist.txt"

// void print_ht(hashmap_t ht[]){
//     for(int i=0;i<HASH_SIZE;i++){
//         printf("bucket %d ", i);
//         node* n = ht[i];
//         if (n != NULL && n->word != NULL){
//             printf("%s ", n->word);
//             while(n != NULL){
//                 n = n->next;
//             }
//         }
//         printf("\n");
//     }
// }
void a(){
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    check_word(correct_word, hashtable);
    check_word(punctuation_word_2, hashtable);
    }
void b(){
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
}

int main(){
    a();
    b();
}