#include "record.h"

/*
 * function: create_record
 * ----------------------------
 * Creates a record ((key, value) pair) for entry into a hash table
 *
 *	key: entry's key
 *	key_size: size of key in bytes
 *  value: value for entry
 *	value_size: size of value bytes
 *
 * returns: a pointer to the record
 */
Record* create_record(const void* key,
                      const size_t key_size,
                      const void* value,
                      const size_t value_size) {

	// Allocate memory for the things
	Record* record = (Record*) malloc(sizeof(Record));

	record->key = malloc(key_size);
	record->value = malloc(value_size);

	// Copy the things
	memcpy(record->key, key, key_size);
	memcpy(record->value, value, value_size);

	return record;
}

/*
 * function: delete_record
 * ----------------------------
 * Frees the memory associated with a record in a table
 *
 *	record: record to delete
 */
void delete_record(Record* record) {
	free(record->key);
	free(record->value);
	free(record);
}
