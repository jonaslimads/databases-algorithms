#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bplus_tree.h"

void initialize_node(node **n, char *tipo) {
	*n = malloc(sizeof(node));

	strcpy((*n)->tipo, tipo);

	(*n)->p[0] = (*n)->p[1] = (*n)->p[2] = NULL;
	(*n)->key[0] = (*n)->key[1] = NULL;

	(*n)->next_free_p = (*n)->next_free_key = 0;
}

int sort_compare_keys(const void *k1, const void *k2) {
	return (*(key**)k1)->value - (*(key**)k2)->value;
}

node *insert(node **root, node *n, int level, int key_value, int rid) {
	
	if (strcmp(n->tipo, "EInd") == 0) {

		// select the subtree
		// compare key_value with all existing keys;
		// break if it found a key_value < n->key[i]
		int i;
		for (i = 0; i < n->next_free_key; i++) {
			if (key_value < n->key[i]->value)
				break;
		}

		// and use i to select the subtree

		node *new_child = insert(
			root, n->p[ i ], level+1,
			key_value, rid
		);

		if (new_child != NULL) {

			// if there is space in n->key[], append this new child
			if (n->next_free_key < 2) {

				n->p[n->next_free_p++] = new_child;

				// get first leaf's key from left
				while (strcmp(new_child->tipo, "EDad") != 0)
					new_child = new_child->p[0]; // going down on the tree
				
				n->key[ n->next_free_key++ ] = new_child->key[0];
				
			} else {
				// we will have n = [A* B* C*], which is not supported
				// so split N into N and N2

				// N2 keeps last D keys and d+1 pointers
				node *n2;
				initialize_node(&n2, "EInd");

				n2->key[0] = new_child->key[0];
				n2->p[0] = n->p[2];
				n2->p[1] = new_child;
				n2->next_free_key = 1;
				n2->next_free_p = 2;

				// N keeps first D keys and d+1 pointers

				key *b = n->p[2]->key[0]; // save it to use later
				n->key[1] = NULL;
				n->p[2] = NULL;
				n->next_free_key--;
				n->next_free_p--;

				
				 // if n is root, we update the root
				if (level == 0) {
					node *new_root;
					initialize_node(&new_root, "EInd");

					new_root->key[new_root->next_free_key++] = b;
					new_root->p[0] = n;
					new_root->p[1] = n2;
					new_root->next_free_p = 2;

					*root = new_root;
					return NULL;
				}

				// if not root, return n2 since n is already updated
				return n2;

			}
		}


	} else { // leaf

		// create new key
		key *new_key = malloc(sizeof(key));
		new_key->value = key_value;
		new_key->rid = rid;

		// if there is free space in n->key[], append it
		if (n->next_free_key < 2) {
			n->key[ n->next_free_key++ ] = new_key;

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
			n->next_free_key = 1;

			node *L2;
			initialize_node(&L2, "EDad");
			L2->key[0] = aux[1];
			L2->key[1] = aux[2];
			L2->next_free_key = 2;

			// return L2 to be insert into this node's parent
			return L2;
		}
	}
	return NULL;
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