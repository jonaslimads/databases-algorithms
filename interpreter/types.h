//typedef enum { STRING, IDENTIFIER, INTEGER } TerminalType;

#include "y.tab.h"

#ifndef TYPES_H
# define TYPES_H


typedef struct terminal {
	enum yytokentype type;

	struct terminal *next; 

	union {
		int integer;
		char *string;
		char *id;
	};
} terminal;

typedef struct tableColumnDefinition {
	char name[21];
	enum yytokentype type;
	int size;

	struct tableColumnDefinition *next;

} tableColumnDefinition;

typedef struct tableSchema {
	unsigned short no_cols;
	unsigned int no_rows;
	unsigned long data_offset; // where records start being written
	struct tableColumnDefinition *columns;
} tableSchema;

#endif