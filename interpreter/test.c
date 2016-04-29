#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "types.h"

#define PREFIX	"storage/"
#define SUFIX	".dat"

void print_table_columns(tableColumnDefinition *col) {
	while (col != NULL) {
		printf("%s %d %d\n", col->name, col->type, col->size);
		col = col->next;
	}
}

int main() {

	// create_table("jonas");
	

	tableColumnDefinition col1 = {"id", INT, 0, NULL};
	tableColumnDefinition col2 = {"name", VARCHAR, 100, NULL};

	tableColumnDefinition *c, *d;
	c = &col1;
	d = &col2;

	c->next = d;

	// print_table_columns(c);

	create_table_schema("jonas", c);

	load_table_schema("jonas");
	// print_table_columns(d);

	// char *name = get_table_filename("books");
	// FILE *f = fopen(name, "wb+");

	// unsigned short no_cols = 2;
	// unsigned int no_rows = 20;
	// unsigned long data_offset = 1231208;

	// // starting
	// fwrite(&no_cols, 1, sizeof(unsigned short), f);
	// fwrite(&no_rows, 1, sizeof(unsigned int), f);
	// fwrite(&data_offset, 1, sizeof(unsigned long), f);

	// unsigned short a;
	// unsigned int b;
	// unsigned long c;
	// fseek(f, 0, SEEK_SET);
	// //fseek(f, 0, SEEK_CUR);
	// fread(&a, sizeof(a), 1, f);
	
	// fread(&b, sizeof(b), 1, f);
	// fread(&c, sizeof(c), 1, f);

	// printf("%d %d %lu\n", a, b, c);

	// b = 30123;
	// fseek(f, sizeof(a), SEEK_SET);
	// fwrite(&b, 1, sizeof(b), f);

	// fseek(f, 0, SEEK_SET);
	// //fseek(f, 0, SEEK_CUR);
	// fread(&a, sizeof(a), 1, f);
	
	// fread(&b, sizeof(b), 1, f);
	// fread(&c, sizeof(c), 1, f);
	// printf("%d %d %lu\n", a, b, c);
	
	return 0;
}