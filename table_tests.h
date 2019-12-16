#ifndef TABLE_TESTS_H_INCLUDED
#define TABLE_TESTS_H_INCLUDED

#include "chained_table.h"
#include <string>

// TEST METHODS //
void test_false(const char*, bool);
void test_true(const char*, bool);
void test_null(const char*, void*);
void test_longs(const char*, long, long);

// ADDREESS BOOK ACCESSORY METHODS //
size_t hash_string(const void*);
int compare_strings(const void*, const void*);
char* address_book_entry_to_string(const void*);

// HELPERS //
HashTable* empty_table();
bool fill_table(HashTable*);

// INSERT TESTS //
void add_to_null_table_fails();
void add_null_key_fails();
void add_null_value_fails();
void test_build_full_table();

// SEARCH TESTS //
void search_null_table_fails();
void search_null_key_fails();
void search_empty_table_fails();
void failed_search_fails();
void normal_search_works();

// REMOVE TESTS //
void remove_from_null_fails();
void remove_null_key_fails();
void remove_key_not_present_fails();
void normal_remove_works();

// REPLACE TESTS //
void replace_in_null_table_fails();
void replace_null_key_fails();
void replace_null_value_fails();
void replace_key_not_present_fails();
void normal_replace_works();

// COMBINATION TESTS //
void add_duplicate_key_fails();
void test_remove_then_add_full_table();

// GROUPINGS //
void insert_tests();
void search_tests();
void remove_tests();
void replace_tests();
void combo_tests();

#endif
