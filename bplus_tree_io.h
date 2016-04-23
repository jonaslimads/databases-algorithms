/**
	This file handles B+ records and indices
	for writing to disk and reading from disk.
*/

#ifndef BPLUS_IO
#define BPLUS_IO

#include "bplus_tree.h"

/**
	Struct for a plain record (as specified by the project).
	It can be stored on/read from disk
*/
typedef struct record {
	char vcod[7];
	char produtor[24];
	char cepa[15];
	int colheita;
	char pais[15];
} record;

/**
	*node and node->key will be transformed into this struct
	so it can be stored on/read from disk.
*/
typedef struct index_storage {
	int key1, key2;
	int rid1, rid2, rid3;
	char *tipo;
} index_storage;

/**
	Store *records on disk
*/
void write_records_data(FILE *f, record *records, int num_records);

/**
	Load a record with rid from disk.
	@param rid 	"rid"th record on the file. It starts from 1
*/
record load_record_data(FILE *f, int rid);

/**
	Using B+ insert algorithm, it generates B+ tree for *records
	Then serialize those B+ indices in a array of *nodes
	that will be used to create a index_storage[].
	index_storage[] is stored on disk finally.
*/
void write_indices_data(FILE *f, record *records, int num_records);

/**
	Load a index with rid from disk.
	@param rid 	"rid"th index on the file. It starts from 1
*/
index_storage load_index_data(FILE *f, int rid);

/**
	Get *node properties and passed it to index_storage.
	node's children are visited to get children's rid
*/
index_storage prepare_node_to_store(node *node);

#endif //BPLUS_IO