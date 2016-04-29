#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define PREFIX	"storage/"
#define SUFIX	".dat"

char *get_table_filename(char *table_name) {
	char *filename = malloc(sizeof(table_name) + sizeof(PREFIX) + sizeof(SUFIX));

	strcpy(filename, PREFIX);
	strcat(filename, table_name);
	strcat(filename, SUFIX);
	
	return filename;
}



/** table file structure:
	no_cols, no_rows, data_offset, COLUMN_DEFINITIONS[], RECORDS[]
	*/ 
int create_table(char *table_name, tableColumnDefinition *col) {

	char *filename = get_table_filename(table_name);

	if (access(filename, F_OK) != -1)
	{
		printf("The table already exists.\n");
		free(filename);

		return -1;
	}
	

	/** CREATE FILE
	*/
	FILE *f;
	f = fopen(filename, "wb+");


	/****
	 	* CREATE SCHEMA
		*/
	unsigned short no_cols = 0;
	unsigned int no_rows = 0;
	unsigned long data_offset = 0; // where records start being written

	fwrite(&no_cols, 1, sizeof(unsigned short), f);
	fwrite(&no_rows, 1, sizeof(unsigned int), f);
	fwrite(&data_offset, 1, sizeof(unsigned long), f);

	// each columns has a name, type and size, so we sum all of it
	int col_total_size = sizeof(col->name) + sizeof(col->type) + sizeof(col->size);


	// need to write each element of the struct once col is a pointer
	while (col != NULL) {
		fwrite(&col->name, 1, sizeof(col->name), f);
		fwrite(&col->type, 1, sizeof(col->type), f);
		fwrite(&col->size, 1, sizeof(col->size), f);

		col = col->next;
		no_cols++;
	}

	// rewrite number of columns
	fseek(f, 0, SEEK_SET);
	fwrite(&no_cols, 1, sizeof(unsigned short), f);

	// rewrite data offset
	data_offset = col_total_size + sizeof(no_cols) + sizeof(no_rows) + sizeof(data_offset);
	fseek(f, sizeof(no_rows), SEEK_CUR);
	fwrite(&data_offset, 1, sizeof(unsigned long), f);


	/***
	 * CLOSING
	*/
	
	printf("The table '%s' has been created.\n", table_name);

	fclose(f);
	free(filename);

	return 0;
}

tableSchema *load_table_schema(char *table_name) {
	char *filename = get_table_filename(table_name);

	FILE *f = fopen(filename, "rb");

	// save up to schema
	tableSchema *schema = malloc(sizeof(tableSchema));
	fread(&schema->no_cols, sizeof(schema->no_cols), 1, f);
	fread(&schema->no_rows, sizeof(schema->no_rows), 1, f);
	fread(&schema->data_offset, sizeof(schema->data_offset), 1, f);

	// need to read each element of tableColumnDefinition
	schema->columns = malloc(schema->no_cols * sizeof(tableColumnDefinition));
	
	for (int i = 0; i < schema->no_cols; i++) {
		fread(schema->columns[i].name, sizeof(schema->columns[i].name), 1, f);
		fread(&schema->columns[i].type, sizeof(schema->columns[i].type), 1, f);
		fread(&schema->columns[i].size, sizeof(schema->columns[i].size), 1, f);
	}

	free(filename);

	return schema;
}

int delete_table(char *table_name) {

	char *filename = get_table_filename(table_name);
	int r = remove(filename);

	free(filename);

	if(r == 0)
		printf("Table deleted successfully\n");
	else 
		printf("Error: unable to delete the table. The table file may not exist already.\n");
   
   return r;
}

void describe_table(char *table_name) {
	tableSchema *schema = load_table_schema(table_name);

	printf("%d %d %lu\n", schema->no_cols, schema->no_rows, schema->data_offset);

	for (int i = 0; i < schema->no_cols; i++) {
		printf("%s %d %d\n", schema->columns[i].name, schema->columns[i].type, schema->columns[i].size);
	}
}

void show_tables() {
	printf("Soon =)\n");
}