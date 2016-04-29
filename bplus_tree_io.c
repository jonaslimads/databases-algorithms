#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bplus_tree_io.h"
#include "bplus_tree.h"

void write_records_data(FILE *f, record *records, int num_records) {
	fwrite(records, sizeof(record), num_records, f);
}

// fseek moves the file pointer to offset position
record load_record_data(FILE *f, int rid) {
	record r;

	if (rid < 1) {
		printf("Error: rid can't be less than 1.\n");
		return r;
	}

	fseek(f, (rid-1)*sizeof(record), SEEK_SET);
	fread(&r, sizeof(record), 1, f);

	return r;
}

void write_indices_data(FILE *f, record *records, int num_records) {
	node *n, *n_leaf; // indices

	// initialize first indice and first leaf, respectively
	initialize_node(&n, "EInd");
	initialize_node(&n_leaf, "EDad");
	n->p[n->next_free_p++] = n_leaf;

	// root is a pointer to the first element. It can be changed later on
	node **root = &n;

	for (int i = 0; i < num_records; i++)
		insert(root, *root, 0, records[i].colheita, i+1);


	print_tree(*root, "");


	// QUEUE ALGORITHM
	// serialize the tree into an array[]

	index_storage storage[1000];
	node *q[1000];
	int front = 0, back = 0;

	q[back] = (*root);
	q[back]->rid = back;
	back++;
	
	while (front < back) {
		// dequeue
		n = q[front++];

		if (n == NULL)
			continue;

		// add to the queue each child if they are not null
		if (n->p[0] != NULL) {
			q[back] = n->p[0];
			q[back]->rid = back;
			back++;
		}

		if (n->p[1] != NULL) {
			q[back] = n->p[1];
			q[back]->rid = back;
			back++;
		}

		if (n->p[2] != NULL) {
			q[back] = n->p[2];
			q[back]->rid = back;
			back++;
		}

	}

	// once we have all nodes on q[], we can move them to
	// storage[] and include children rid
	for (int i = 0; i < back; i++)
		storage[i] = prepare_node_to_store(q[i]);

	// now we can store storage[] into the FILE
	fwrite(storage, sizeof(index_storage), back, f);
	
}


int search(FILE *f, int key_value) {

	// initially root
	index_storage *i = malloc(sizeof(index_storage));
	//i->boolean_index_visited = 0;

	fseek(f, 0, SEEK_SET);
	fread(i, sizeof(index_storage), 1, f);

	printf("bolean %d\n", i->boolean_index_visited);
	while (i != NULL) {
		if((strcmp(i->tipo, "EDad") == 0) && (i->boolean_index_visited == 0)) {

			if(i->key1 == key_value){
				i->boolean_index_visited = 1;
				return i->rid1;
			}
			else if(i->key2 == key_value){
				i->boolean_index_visited = 1;
				return i->rid2;
			}
			else{
				//colocar ou nao?
				return -1;
			}
		} else {

			if(key_value < i->key1)
				fseek(f, (i->rid1-1)*sizeof(index_storage), SEEK_SET);
			else if(key_value >= i->key2)
				fseek(f, (i->rid3-1)*sizeof(index_storage), SEEK_SET);
			else
				fseek(f, (i->rid2-1)*sizeof(index_storage), SEEK_SET);

			fread(i, sizeof(index_storage), 1, f);
		}
	}

	return -1;

}


void search2(FILE *f, int key_value){
	int vector[20];
	int j = 0;
	
	int item_rid;
	printf("teste1\n");
	do{
		item_rid = search(f,key_value);
		if(item_rid != -1){
			vector[j] = item_rid;
			j++;
			printf("teste2\n");
		}


	}while(item_rid != -1);


	printf("teste3\n");
	for(int i = 0; i < j; i++){
		printf("%d\n", vector[i]);
		printf("teste4\n");
	}
}


//put all values of index_storage.boolean_index_visited = 0
void refresh(FILE *f){


}

index_storage load_index_data(FILE *f, int rid) {
	index_storage i;

	if (rid < 1) {
		printf("Error: rid can't be less than 1.\n");
		return i;
	}

	fseek(f, (rid-1)*sizeof(index_storage), SEEK_SET);
	fread(&i, sizeof(index_storage), 1, f);

	return i;
}

index_storage prepare_node_to_store(node *node) {
	index_storage a;
	a.tipo = node->tipo;

	a.key1 = node->key[0] != NULL ? node->key[0]->value : -1;
	a.key2 = node->key[1] != NULL ? node->key[1]->value : -1;

	a.boolean_index_visited = 0;
	
	if (strcmp(a.tipo, "EInd") == 0) {
		// indices start from 1 (not 0)
		a.rid1 = node->p[0] != NULL ? node->p[0]->rid+1 : -1;
		a.rid2 = node->p[1] != NULL ? node->p[1]->rid+1 : -1;
		a.rid3 = node->p[2] != NULL ? node->p[2]->rid+1 : -1;
	} else { // EDad
		a.rid1 = node->key[0] != NULL ? node->key[0]->rid : -1;
		a.rid2 = node->key[1] != NULL ? node->key[1]->rid : -1;
		a.rid3 = -1;
	}
	return a;
} 
