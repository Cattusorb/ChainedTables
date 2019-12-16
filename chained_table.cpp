#include "chained_table.h"

/*
 * function: create_table
 * ----------------------------
 * Creates a hash table using open addressing and lazy deletion
 *	N: table size
 *	hash_function: hash function used to find bucket numbers for keys
 *	compare: function for comparing keys. Must obey the following rules:
 *			compare(a, b) < 0 if a < b
 *			compare(a, b) > 0 if a > b
 *			compare(a, b) = 0 if a == b
 *	record_formatter: returns a C string (char*) representation of a record
 *	key_size: size of keys in bytes
 *	value_size: size of values in bytes
 *
 * returns: a pointer to the table
 *
 */
HashTable* create_table(const int N,
						size_t (*hash_function)(const void*),
						int (*compare)(const void*, const void*),
						char* (*record_formatter)(const void*),
						const size_t key_size, const size_t value_size) {
    // Allocate space for the table
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    // Initialize attributes
    table->N = N;
    table->hash_function = hash_function;
    table->compare = compare;
    table->record_formatter = record_formatter;
    table->key_size = key_size;
    table->value_size = value_size;
    // Initialize records - this is an array of Records
    // Initially, all the buckets will be empty
    table->buckets = (LinkedList**) malloc(N * sizeof(LinkedList*));
    for (int i = 0; i < N; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

/*
 * function: table_to_string
 * ----------------------------
 * Provides a string representation of the table
 *	table: the table to return in string form
 *
 * returns: a string representation of the table
 *
 */
char* table_to_string(const HashTable* table) {
	char* out = (char*) malloc(2048 * sizeof(char));
	sprintf(out, "N\tB[n]\n----------------");

	for (int i = 0; i < table->N; i++) {
		char* holder = get_string(table->buckets[i], table->record_formatter);
		sprintf(out, "%s\n%d\t%s", out, i, holder);
	}

	return out;
}


/*
 * function: insert
 * ----------------------------
 * Inserts a new entry into the table
 *	table: the table to add to
 *  key: key to add
 *  value: corresponding value
 *
 */
bool insert(HashTable* table, const void* key, const void* value) {
    if (table == NULL || key == NULL || value == NULL)
    {
        return false;
    }

	Record* r;
	r->key = (void*)key;
	r->value = (void*)value;
	add_beginning(table->buckets[table->hash_function(key) % table->N], r, sizeof(Record*));
	return true;
}

/*
 * function: search
 * ----------------------------
 * Searches the table for a particular key and returns the corresponding value
 *	table: the table to search
 *  key: key to search for
 *
 * returns: the value or NULL if key is not in table
 */
void* search(const HashTable* table, const void* key) {
    if (table == NULL || key == NULL)
    {
        return NULL;
    }

	Node* node = table->buckets[table->hash_function(key) % table->N]->first;

	while (node != NULL)
    {
        Record* r = (Record*)node->data;
        if (table->compare(key, r->key) == 0)
        {
            return r->value;
        }
	}

	return NULL;
}

/*
 * function: replace
 * ----------------------------
 * Replaces the value of the entry with a given key in the table
 *	table: the table to add to
 *  key: key to add
 *  new_value: new value for key
 *
 */
bool replace(HashTable* table, const void* key, const void* new_value) {
    if (table == NULL || key == NULL || new_value == NULL)
    {
        return false;
    }

	Node* node = table->buckets[table->hash_function(key) % table->N]->first;

    while (node != NULL)
    {
        Record* r = (Record*)node->data;
        if (table->compare(key, r->key) == 0)
        {
            r->value = (void*)new_value;
            return true;
        } else
        {
            node = node->next;
        }
    }

    return false;
}

/*
 * function: remove
 * ----------------------------
 * Removes an entry from the table using lazy deletion
 *	table: the table to remove the entry from
 *  key: key of entry to remove
 *
 */
bool remove(HashTable* table, const void* key) {
    if (table == NULL || key == NULL)
    {
        return false;
    }

	int count = 0;
	Node* node = table->buckets[table->hash_function(key) % table->N]->first;

    while (node != NULL)
    {
        Record* r = (Record*)node->data;
        if (table->compare(key, r->key) == 0)
        {
            break;
        } else
        {
            count = count + 1;
            node = node->next;
        }
    }

    if (node != NULL)
    {
        remove_n(table->buckets[table->hash_function(key) % table->N], count);
        return true;
    } else
    {
        return false;
    }
}
