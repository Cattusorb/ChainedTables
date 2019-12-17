using namespace std;

#include "table_tests.h"

/********************************* CONSTANTS *********************************/
const string NAMES[] = {"Alice", "Bob", "Chris", "Dave", "Emily", "Frank",
						"Grace", "Harry", "Ingrid", "Jeremy"};
const long NUMBERS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

const string KATE = "Kate";
const long KATE_NUMBER = 11;

const string JEREMY = "Jeremy";
const long JEREMY_NUMBER = 10;

/******************************** TEST METHODS ********************************/

/*
 * Function: test_false
 * ----------------------------
 * runs a test where the expected output is false
 *
 */
void test_false(const char* test_name, bool result) {
	printf("Test: %s... ", test_name);

	if (!result) {
		printf("passed\n");
	} else {
		printf("FAILED.");
	}
}

/*
 * Function: test_true
 * ----------------------------
 * runs a test where the expected output is true
 *
 */
void test_true(const char* test_name, bool result) {
	printf("Test: %s... ", test_name);

	if (result) {
		printf("passed\n");
	} else {
		printf("FAILED.");
	}
}

/*
 * Function: test_NULL
 * ----------------------------
 * runs a test where the expected output is NULL
 *
 */
void test_null(const char* test_name, void* result) {
	printf("Test: %s... ", test_name);

	if (result == NULL) {
		printf("passed\n");
	} else {
		printf("FAILED.");
	}
}

/*
 * Function: test_with_longs
 * ----------------------------
 * runs a test where the expected output is a long
 *
 */
void test_longs(const char* test_name, const long result, const long expected) {
	printf("Test: %s... ", test_name);

	if (result == expected) {
		printf("passed\n");
	} else {
		printf("FAILED. Expected %ld (result %ld)\n", expected, result);
	}
}

/*
 * function: hash_string
 * ----------------------------
 * Uses std::hash to calculate the hash of a given string
 *	aString: string to hash
 *
 * returns: hash of the string
 *
 */
size_t hash_string(const void* aString) {
	string s = *((string*) aString);
	return hash<string>{}(s);
}

/*
 * function: compare_strings
 * ----------------------------
 * Uses the std::string class to compare two strings
 *	p1: pointer to a string
 *  p2: pointer to another string
 *
 * returns: -1 if string at p1 comes before string at p2 in lexicographical order
 *          +1 if string at p1 comes after string at p2 in lexicographical order
 *		     0 if strings at p1 and p2 are equivalent
 *
 */
int compare_strings(const void* p1, const void* p2) {
	string s1 = *((string*) p1);
	string s2 = *((string*) p2);

	return s1.compare(s2);
}


/*
 * function: address_book_entry_to_string
 * ----------------------------
 * Returns a char* representation of a record
 *
 * 	p: pointer to a record
 */
char* address_book_entry_to_string(const void* p) {
	Record* record = (Record*) p;
	string name = *((string*) record->key);
	long number = *((long*) record->value);

	int area_code = number / 10000000;
	int exchange = (number % 10000000) / 10000;
	int last_four = number % 10000;

	char* buff = (char*) malloc(256 * sizeof(char));
	sprintf(buff, "%s: (%03d) %03d-%04d", name.c_str(), area_code, exchange, last_four);

	return buff;
}

/********************************** HELPERS **********************************/
HashTable* empty_table() {
	HashTable* table = create_table(10, hash_string, compare_strings,
									address_book_entry_to_string,
									sizeof(string), sizeof(long));

	return table;
}

bool fill_table(HashTable* table) {
	bool success =  true;
	for (int i = 0; i < 10; i++) {
		success = success && insert(table, &NAMES[i], &NUMBERS[i]);
	}

	return success;
}

/********************************** INSERT **********************************/
void add_to_null_table_fails(){
	test_false("Can't add to NULL table", insert(NULL, &KATE, &KATE_NUMBER));
}

void add_null_key_fails(){
	HashTable* table = empty_table();
	test_false("Can't add NULL key", insert(table, NULL, &KATE_NUMBER));
}

void add_null_value_fails(){
	HashTable* table = empty_table();
	test_false("Can't add NULL value", insert(table, &KATE, NULL));
}

void add_duplicate_key_fails() {
	HashTable* table = empty_table();
	fill_table(table);
	test_false("Add duplicate key fails", insert(table, &JEREMY, &JEREMY_NUMBER));
}

void test_build_full_table(){
	HashTable* table = empty_table();
	test_true("Successful build of full table", fill_table(table));
}

/********************************** SEARCH **********************************/
void search_null_table_fails() {
	test_null("Can't search NULL table", search(NULL, &KATE));
}

void search_null_key_fails() {
	HashTable* table = empty_table();
	fill_table(table);
	test_null("Can't search for NULL key", search(table, NULL));
}

void search_empty_table_fails() {
	HashTable* table = empty_table();
	test_null("Searching an empty table fails", search(table, &KATE));
}

void failed_search_fails() {
	HashTable* table = empty_table();
	fill_table(table);
	test_null("Failed search fails", search(table, &KATE));
}

void normal_search_works() {
	HashTable* table = empty_table();
	fill_table(table);
	long* lookup = (long*) search(table, &JEREMY);
	test_longs("Normal lookup works", *lookup, JEREMY_NUMBER);
}

/********************************** REMOVE **********************************/
void remove_from_null_fails() {
	test_false("Can't remove from NULL table", remove(NULL, &JEREMY));
}

void remove_null_key_fails() {
	HashTable* table = empty_table();
	fill_table(table);
	test_false("Can't remove NULL key", remove(table, NULL));
}

void remove_key_not_present_fails() {
	HashTable* table = empty_table();
	fill_table(table);

	test_false("Can't remove key not in table", remove(table, &KATE));
}

void normal_remove_works() {
	HashTable* table = empty_table();
	fill_table(table);

	if (remove(table, &JEREMY)) {
		test_null("Key successfully removed from table", search(table, &JEREMY));
	} else {
		// Print a failure
		test_true("Removal operation works", false);
	}
}

/********************************** REPLACE **********************************/
void replace_in_null_table_fails() {
	test_false("Can't replace in NULL table", replace(NULL, &JEREMY, &KATE_NUMBER));
}

void replace_null_key_fails() {
	HashTable* table = empty_table();
	fill_table(table);
	test_false("Can't replace with NULL key", replace(table, NULL, &KATE_NUMBER));
}

void replace_null_value_fails() {
	HashTable* table = empty_table();
	fill_table(table);
	test_false("Can't replace with NULL value", replace(table, &JEREMY, NULL));
}

void replace_key_not_present_fails() {
	HashTable* table = empty_table();
	fill_table(table);

	test_false("Can't replace key not in table", remove(table, &KATE));
}

void normal_replace_works() {
	HashTable* table = empty_table();
	fill_table(table);

	if (replace(table, &JEREMY, &KATE_NUMBER)) {
		// Replace says it did things right, let's check
		long* number = (long*) search(table, &JEREMY);
		test_longs("Replacement successful", *number, KATE_NUMBER);
	} else {
		// Print a failure
		test_true("Replacement operation works", false);
	}
}

/******************************** COMBINATIONS ********************************/
void test_remove_then_add_full_table() {
	HashTable* table = empty_table();
	fill_table(table);

	remove(table, &JEREMY);

	if (insert(table, &KATE, &KATE_NUMBER)) {
		long* number = (long*) search(table, &KATE);
		test_longs("Entry added after delete from full table", *number, KATE_NUMBER);
	} else {
		// Fail message
		test_true("Add operation after delete from full table", false);
	}
}

/******************************** TEST GROUPS ********************************/
void insert_tests() {
	add_to_null_table_fails();
	add_null_key_fails();
	add_null_value_fails();
	add_duplicate_key_fails();
	test_build_full_table();
}

void search_tests() {
	search_null_table_fails();
	search_null_key_fails();
	search_empty_table_fails();
	failed_search_fails();
	normal_search_works();
}

void remove_tests() {
	remove_from_null_fails();
	remove_null_key_fails();
	remove_key_not_present_fails();
	normal_remove_works();
}

void replace_tests() {
	replace_in_null_table_fails();
	replace_null_key_fails();
	replace_null_value_fails();
	replace_key_not_present_fails();
	normal_replace_works();
}

void combo_tests() {
	test_remove_then_add_full_table();
}
