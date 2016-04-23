#ifndef BPLUS_TREE
#define BPLUS_TREE

/***
	This algorithm works for B+ tree with D = 1
	*/

/*** 
	Structs for indices
	*/
typedef struct node {
	char tipo[5];
	struct node *p[3]; // children pointer; used when tipo == "EInd"
	struct key *key[2];
	int next_free_p; // next free pointer space; initially 0
	int next_free_key; // next free key space; initially 0
	int rid; // represents its rid when the indices are stored in the file 
} node;

// rid is an element index from records *records
typedef struct key {
	int value;
	int rid; // used only when the node is tipo == "EDad"
} key;


/***
	Allocate memory for a node *n;
	set all its pointers and keys as null.
	*/
void initialize_node(node **n, char *tipo);

/***
	reference: http://stackoverflow.com/questions/23689687/sorting-an-array-of-struct-pointers-using-qsort

	qsort() comparison function
	It compares two keys' value.

	*/
int sort_compare_keys(const void *k1, const void *k2);


/**
	@param **root	the tree root itself; always points to
					the first index on the tree
	@param *n 	the first index pointed by *root
				the recursion will call a n->child
	@param level	current level on the tree. It increases for each recursion
					initially it's 0.
	@param key_value	key to be inserted
	@param rid	key rid in the data file

	*/
node *insert(node **root, node *n, int level, int key_value, int rid);

/**
	Print a tree whose first index is *n;
	*str is the prefix for each line.
*/
void print_tree(node *n, char *str);

#endif // BPLUS_TREE