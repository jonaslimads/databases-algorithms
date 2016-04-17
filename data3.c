#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define D 1


typedef struct record {
	char vcod[7];
	char produtor[24];
	char cepa[15];
	int colheita;
	char pais[15];
} record;


/**** 
	Structs for indices
*****/
typedef struct node {
	char tipo[5];
	struct node *p[3]; // children pointer; used when tipo == "EInd"
	struct key *key[2];
	int next_free_p; // next free pointer space; initially 0
	int next_free_key; // next free key space; initially 0
} node;

// rid is an element index from records *records
typedef struct key {
	int value;
	int rid; // used only when the node is tipo == "EDad"
} key;

void write_records_data(FILE *f, record *records, int num_records) {
	fwrite(records, sizeof(record), num_records, f);
}

// fseek moves the file pointer to offset position
void load_record_data(FILE *f, long offset) {
	record r;

	fseek(f, offset, SEEK_SET);
	fread(&r, sizeof(r), 1, f);

	printf("%s %s\n", r.vcod, r.cepa);
}

void print_tree(node *n, char *str) {
	if (n == NULL)
		return;

	char *str2 = malloc(sizeof(str) + 2*sizeof(char));
	strcpy(str2, str);
	strcat(str2, "\t");

	print_tree(n->p[0], str2);

	if (n->key[0] != NULL)
		printf("%skey[%d] = %d\n", str, 0, n->key[0]->value);
	else
		printf("%skey[%d] = _\n", str, 0);

	print_tree(n->p[1], str2);

	if (n->key[1] != NULL)
		printf("%skey[%d] = %d\n", str, 1, n->key[1]->value);
	else
		printf("%skey[%d] = _\n", str, 1);

	print_tree(n->p[2], str2);
}

/*********
	B+ INSERT
**********/

void initialize_node(node *n, char *tipo) {
	strcpy(n->tipo, tipo);

	// n->key[0] = malloc(sizeof(key));
	// n->key[1] = malloc(sizeof(key));
	// n->key[0]->value = n->key[0]->value
	// 	= n->key[0]->value = n->key[0]->value = -1;

	n->p[0] = n->p[1] = n->p[2] = NULL;
	n->key[0] = n->key[1] = NULL;

	n->next_free_p = n->next_free_key = 0;
}

// http://stackoverflow.com/questions/23689687/sorting-an-array-of-struct-pointers-using-qsort
int sort_compare_keys(const void *k1, const void *k2) {
	return (*(key**)k1)->value - (*(key**)k2)->value;
}

node *insert(node *n, int key_value, int rid, node *new_node_recursion) {
	if (strcmp(n->tipo, "EInd") == 0) {

		node *new_child = insert(
			n->p[ n->next_free_key ],
			key_value, rid, new_node_recursion
		);

		if (new_child != NULL) {

			// if there is space in n->key[], append this new child
			if (n->next_free_key < 2) {
				n->key[ n->next_free_key ] = new_child->key[0];
				n->p[n->next_free_p] = new_child;
				n->next_free_p++;

			} else {

			}
		}

		return NULL;

	} else { // leaf

		// create new key
		key *new_key = malloc(sizeof(key));
		new_key->value = key_value;
		new_key->rid = rid;

		// if there is free space in n->key[], append it
		if (n->next_free_key < 2) {
			n->key[ n->next_free_key ] = new_key;
			n->next_free_key++;

			// in case there is more than one key, sort n->key[]
			if (n->next_free_key >= 2)
				qsort(n->key, n->next_free_key, sizeof(key*), sort_compare_keys);

		} else { // out of space
			/* example: [17* 19*] is already in the leaf (max 2 keys); 
				we want to append 16*, so we need to sort [17* 19* 16*]
				then redistribute it into two leaves: [16* NULL] and [17* 19*]
				*/
			key *aux[3];
			aux[0] = n->key[0];
			aux[1] = n->key[1];
			aux[2] = new_key;

			qsort(aux, 3, sizeof(key*), sort_compare_keys);

			// split leaf: first d keys stay in L; d+1 move to L2
			n->key[0] = aux[0];
			n->key[1] = NULL;

			node *L2 = malloc(sizeof(node));
			initialize_node(L2, "EDad");
			L2->key[0] = aux[1];
			L2->key[1] = aux[2];

			// return L2 to be insert into this node's parent
			return L2;
		}
		return NULL;
	}
	return NULL;
}

void write_indices_data(FILE *f, record *records, int num_records) {
	node *n; // indices
	n = malloc(100*sizeof(node));
	
	// initialize root and first leaf, respectively
	initialize_node(&n[0], "EInd");
	n[0].p[0] = &n[1];
	n[0].next_free_p++;
	
	initialize_node(&n[1], "EDad");

	insert(n, records[0].colheita, 0, NULL);
	insert(n, records[1].colheita, 1, NULL);
	insert(n, records[2].colheita, 2, NULL);

	print_tree(n, "");
}





int main() {

	static const int num_records = 5;
	
	record r[num_records] = {
		{"V100", "Don Laurindo", "Merlot", 2017, "Brasil"},
		{"V150", "Chateau Lafitte", "Chardonnay", 2016, "Franca"},
		{"V180", "Chryseia", "Douro", 2014, "Portugal"},
		{"V190", "Chryseia", "Douro", 2012, "Portugal"},
		{"V200", "Chryseia", "Douro", 2017, "Portugal"},
	};

	// write
	FILE *f, *i;
	f = fopen("Dados.dat", "wb+");
	i = fopen("Indices.dat", "wb+");


	write_records_data(f, r, num_records);
	write_indices_data(i, r, num_records);


	// load_record_data(f, 0*sizeof(record)); // first record
	// load_record_data(f, 1*sizeof(record)); // second...
	// load_record_data(f, 2*sizeof(record)); // 


	fclose(f);
	return 0;
}