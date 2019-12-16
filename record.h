#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include <stdlib.h>
#include <string.h>

/*
 * struct: Record
 * ----------------------------
 * A (key, value) pair in a hash table
 *
 */
typedef struct Record {
	void* key;
	void* value;
} Record;

Record* create_record(const void*, const size_t, const void*, const size_t);
void delete_record(Record*);

#endif
