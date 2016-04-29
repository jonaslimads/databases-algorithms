#ifndef IO_H
# define IO_H

// accessed from Yacc file
int create_table(char *table_name, tableColumnDefinition *col);
int delete_table(char *table_name);
void describe_table(char *table_name);
void show_tables();
void insert_into(char *table_name);


// not accessed from Yacc file
char *get_table_filename(char *table_name);
tableSchema *load_table_schema(char *table_name);

#endif