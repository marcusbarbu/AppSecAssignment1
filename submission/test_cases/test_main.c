#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    // Here we can test if certain words ended up in certain buckets
    // to ensure that our load_dictionary works as intended. I leave
    // this as an exercise.
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    // Test here: What if a word begins and ends with "?
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert_msg(num_misspelled == 3, "actual %d", num_misspelled);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_words_pnp)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[7];
    expected[0] = "neighbourhood";
    expected[1] = "proparty";
    expected[2] = "Bennet";
    expected[3] = "Netherfield";
    expected[4] = "Bennet";
    expected[5] = "hiere";
    expected[6] = "t'old";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("pnp.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert_msg(num_misspelled == 7, "actual %d", num_misspelled);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0, "actual %s, expected %s", misspelled[0], expected[0]);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0, "actual %s, expected %s", misspelled[1], expected[1]);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0, "actual %s, expected %s", misspelled[2], expected[2]);
    ck_assert_msg(strcmp(misspelled[3], expected[3]) == 0, "actual %s, expected %s", misspelled[3], expected[3]);
    ck_assert_msg(strcmp(misspelled[4], expected[4]) == 0, "actual %s, expected %s", misspelled[4], expected[4]);
    ck_assert_msg(strcmp(misspelled[5], expected[5]) == 0, "actual %s, expected %s", misspelled[5], expected[5]);
    ck_assert_msg(strcmp(misspelled[6], expected[6]) == 0, "actual %s, expected %s", misspelled[6], expected[6]);
}
END_TEST

START_TEST(test_unicode_numbers)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[2];
    char *misspelled[MAX_MISSPELLED];
    expected[0] = "tráffico";
    expected[1] = "這揭開了德";
    FILE* fp = fopen("spanish.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) ==0, "actual %s, expected %s", misspelled[0], expected[0]);
    // fp = fopen("chinese.txt", "r");
    // num_misspelled = check_words(fp, hashtable, misspelled);
    // ck_assert_msg(strcmp(misspelled[0], expected[1]) ==0, "actual %s, expected %s", misspelled[0], expected[1]);
    fp = fopen("phone.txt", "r");  
    num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert_msg(num_misspelled == 0, "actual %d", num_misspelled);
}
END_TEST

START_TEST(test_word_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    bool m = check_word("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", hashtable);
    ck_assert_msg(m == false, "check_word failed");

    char *misspelled[MAX_MISSPELLED];
    FILE* fp = fopen("overflow.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert_msg(num_misspelled == 7, "Failed checking words from file, actual %d", num_misspelled);
}
END_TEST

START_TEST(test_dict_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary("ovwordlist.txt", hashtable);
    bool m = check_word("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", hashtable);
    ck_assert_msg(m == false, "check_word failed");

    char *misspelled[MAX_MISSPELLED];
    FILE* fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert_msg(num_misspelled == 3, "actual %d", num_misspelled);
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_check_words_pnp);
    tcase_add_test(check_word_case, test_unicode_numbers);
    tcase_add_test(check_word_case, test_word_overflow);
    tcase_add_test(check_word_case, test_dict_overflow);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

