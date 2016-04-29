%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "types.h"
	#include "io.h"

	int yylex(void);
	void yyerror(char *);
	//int sym[26];

	// prototypes
	terminal *new_terminal(enum yytokentype type);
	terminal *terminal_int(int i);
	terminal *terminal_id(char *str);
	terminal *terminal_string(char *str);

	void insert_into_table_passing_terminals(char *table_name, terminal *cols, terminal *values,
			int no_insert_into_columns, int no_insert_into_values);
	tableColumnDefinition *create_table_colum_definition(char *column_name, enum yytokentype type, int size);

	// debug
	void print_terminal(terminal *t);
	void print_table_columns(tableColumnDefinition *col);

	int no_table_colum_definition = 0;
	int no_insert_into_columns = 0;
	int no_insert_into_values = 0;

%}

%union {
	int integer;
	char *id;
	char *string;
	struct terminal *t;
	struct tableColumnDefinition *table_column_definition;
}

%token <integer> INTEGER
%token <id> IDENTIFIER
%token <string> STRING

%token CREATE
%token DATABASE
%token DELETE
%token DESCRIBE
%token EXIT
%token FROM
%token INSERT
%token INT
%token INTO
%token QUIT
%token SELECT
%token SHOW
%token TABLE TABLES
%token USE
%token VALUES
%token VARCHAR

%type <t> statement insert_columns
	insert_column_list identifier_with_without_grave_accent
	insert_value_list terminal_valued

%type <table_column_definition> create_table_definition create_table_definitions

%%

program
		: program statement ';'	{ /*printf("%d\n", 2);*/ }
		|
		;

statement
		: CREATE TABLE IDENTIFIER '(' create_table_definitions ')'
				{
					create_table($3, $5);
					$$ = terminal_string($3);
					// print_table_columns($5);
				}
		| DELETE TABLE IDENTIFIER
				{
					delete_table($3);
					$$ = terminal_string($3);
				}
		| DESCRIBE TABLE IDENTIFIER
				{
					describe_table($3);
					$$ = terminal_string($3);
				}
		| SHOW TABLES
				{
					show_tables();
					$$ = terminal_string("t");
				}
		| SELECT identifier_with_without_grave_accent
			FROM identifier_with_without_grave_accent
				{
					$$ = $2;
				}
		| INSERT INTO IDENTIFIER insert_columns  VALUES '(' insert_value_list ')'
				{
					// print_terminal($1); print_terminal($4);
					
					insert_into_table_passing_terminals($3, $4, $7,
						no_insert_into_columns, no_insert_into_values);

					no_insert_into_columns = no_insert_into_values = 0;
					$$ = $4;
				}
		| {}
		;

	/* Create table rules */

create_table_definitions
		: create_table_definition 			{ $$ = $1; }
		| create_table_definition ',' create_table_definitions
				{
					$1->next = $3;
					$$ = $1;
					printf("%d\n", no_table_colum_definition);
				}
		;

create_table_definition
		: identifier_with_without_grave_accent INT 		
				{
					no_table_colum_definition++;
					$$ = create_table_colum_definition($1->id, INT, 0);
				}
		| identifier_with_without_grave_accent VARCHAR '(' INTEGER ')'
				{
					no_table_colum_definition++;
					$$ = create_table_colum_definition($1->id, VARCHAR, $4);
				}
		;

	/* Insert into rules */

insert_columns
		: '(' insert_column_list ')'	{ $$ = $2; }
		| 								{ $$ = NULL; }
		;

insert_column_list
		: identifier_with_without_grave_accent	{ no_insert_into_columns++; $$ = $1; }
		| identifier_with_without_grave_accent ',' insert_column_list
					{
						// linked list of terminals
						// right recursion because appending from tail is O(1)
						no_insert_into_columns++; 
						$1->next = $3;
						$$ = $1;
					}
		;

identifier_with_without_grave_accent
		: IDENTIFIER 				{ $$ = terminal_id($1); }
		| '`' IDENTIFIER '`'		{ $$ = terminal_id($2); }
		;

insert_value_list
		: terminal_valued 			{ no_insert_into_values++; $$ = $1;}
		| terminal_valued ',' insert_value_list
				{
					no_insert_into_values++; 
					$1->next = $3;
					$$ = $1;
				}
		;

terminal_valued
		: STRING 					{ $$ = terminal_string($1); }
		| INTEGER 					{ $$ = terminal_int($1); }
		;

%%

/** Terminals methods
*/
terminal *new_terminal(enum yytokentype type) {
	terminal *t = malloc(sizeof(terminal));
	t->next = NULL;
	t->type = type;
	return t;
}

terminal *terminal_int(int i) {
	terminal *t = new_terminal(INTEGER);
	t->integer = i;
	return t;
}

terminal *terminal_id(char *str) {
	terminal *t = new_terminal(IDENTIFIER);
	t->id = strdup(str);
	return t;
}

terminal *terminal_string(char *str) {
	terminal *t = new_terminal(STRING);
	t->string = strdup(str);
	return t;
}

tableColumnDefinition *create_table_colum_definition(char *column_name, enum yytokentype type, int size) {
	tableColumnDefinition *col = malloc(sizeof(tableColumnDefinition));
	// col->name = strdup(column_name);
	strcpy(col->name, column_name);
	col->type = type;
	col->size = size;
	col->next = NULL;
	return col;
}

void insert_into_table_passing_terminals(char *table_name, terminal *cols, terminal *values,
	int no_insert_into_columns, int no_insert_into_values) {
	printf("%d %d\n", no_insert_into_columns, no_insert_into_values);
	if (no_insert_into_columns == 0) {

	} else if (no_insert_into_columns == no_insert_into_values) {

	} else {
		printf("The number of columns must be 0 or the same as of values.\n");
		return ;
	}
}

void print_terminal(terminal *t) {
	while (t != NULL) {
		switch(t->type) {
			case IDENTIFIER:
				printf("%s\n", t->id);
				break;
			case INTEGER:
				printf("%d\n", t->integer);
				break;
			case STRING:
				printf("%s\n", t->string);
				break;
			default: break;
		}
			
		t = t->next;
	}
}

void print_table_columns(tableColumnDefinition *col) {
	while (col != NULL) {
		printf("%s %d %d\n", col->name, col->type, col->size);
		col = col->next;
	}
}

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

int main(void) {
	yyparse();
	return 0;
}