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

// rid/rid2 can't be a pointer because 
// its data is stored in the disk, not in the memory.
// So rid/rid2 will be used as offset position and they can be "null" (-1)
typedef struct node {
	int key[2];
	struct node *p[3]; // used if tipo == "EInd"
	struct record *r[2]; // used if tipo == "EDad"
	char tipo[5];
	int next_free_ptr;
} node;


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

	printf("%skey[%d] = %d\n", str, 0, n->key[0]);

	print_tree(n->p[1], str2);

	printf("%skey[%d] = %d\n", str, 1, n->key[1]);

	print_tree(n->p[2], str2);
}

void initialize_node(node *new, char *tipo) {
	new->key[0] = new->key[1] = -1;
	strcpy(new->tipo, tipo);
	new->p[0] = new->p[1] = new->p[2] = NULL;
	new->r[0] = new->r[1] = NULL;
	new->next_free_ptr = 0;
}

// iterate through n->key[] to find a free space
// and return its free space index
// if i >= length(n->key), it does not have free space
int search_free_key_space(node *n) {
	int i;	
	for (i = 0; i < 2;) {
		if (n->key[i] == -1)
			break;
		i++;
	}
	if (i >= 2) // no free space
		return -1;
	return i;
}

node *insert(node *n, record r, node *new_node_recursion) {
	if (strcmp(n->tipo, "EInd") == 0) {

		// pick a subtree based on its key
		int i = search_free_key_space(n);

		node *new_child = insert(n->p[i], r, new_node_recursion);

		if (new_child != NULL) {
			int free_space = search_free_key_space(n);
			
			if (free_space > -1) {
				// append child to this node
				n->key[free_space] = new_child->key[0];
				
				n->p[n->next_free_ptr] = new_child;
				n->next_free_ptr++;
			} else {
				// out of space; split node in two
				// node *new_node = malloc(sizeof(node));
				// initialize_node(new_node, "EInd");
				// new_node->key[0] = n->key[1];
				// // new_leaf->key[0] = n->key[1];
				// n->key[1] = -1;
				// new_leaf->p[0] = n->p[2];
				
				// printf("aqui");
			}
		}

	} else { // leaf

		// try to insert in the first free space
		int free_space = search_free_key_space(n);

		int i;	
		for (i = 0; i < 2;) {
			if (n->key[i] == -1)
				break;
			i++;
		}
		
		// out of space; split leaf in two
		if (free_space == -1) {
			node *new_leaf = malloc(sizeof(node));
			initialize_node(new_leaf, "EDad");

			new_leaf->key[0] = n->key[1];
			new_leaf->key[1] = r.colheita;
			n->key[1] = -1;
			// configurar ponteiros irmaos em L e L2

			return new_leaf;
		} else {
			//printf("i %d\n", free_space);
			n->key[free_space] = r.colheita;
			//n->r[free_space] = &r;
			// printf("%d\n", n->key[free_space]);

			return NULL;
		}
		
	}

	return NULL;
}

void write_indices_data(FILE *f, record *records, int num_records) {
	node *n; // indices
	n = malloc(100*sizeof(node));
	
	// initialize root and first leaf, respectively
	initialize_node(&n[0], "EInd");
	n[0].p[0] = &n[1];
	n[0].next_free_ptr++;
	
	initialize_node(&n[1], "EDad");

	insert(n, records[0], NULL);
	insert(n, records[1], NULL);
	insert(n, records[2], NULL);
	insert(n, records[3], NULL);
	// insert(n, records[4], NULL);

	//printf("%d\n", n[1].r[0]->colheita);
	// printf("T: %d\n", (n[0].p[1])->key[0]);
	// printf("T: %d\n", n[1].key[0]);

	print_tree(n, "");
}





int main() {

	static const int num_records = 5;
	
	record r[num_records] = {
		{"V100", "Don Laurindo", "Merlot", 2011, "Brasil"},
		{"V150", "Chateau Lafitte", "Chardonnay", 2010, "Franca"},
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