%{
	#include <stdlib.h>
	void yyerror(char *);
	#include "y.tab.h"
%}

num			[0-9]
alpha		[a-zA-Z_]
alphanum	[a-zA-Z_0-9]
string		\"(\\.|[^"])*\"


%%

	/* reserved words */

"create"	return CREATE;
"database"	return DATABASE;
"delete"	return DELETE;
"describe"	return DESCRIBE;
"exit"		return EXIT;
"from"		return FROM;
"insert"	return INSERT;
"int"		return INT;
"into"		return INTO;
"quit"		return QUIT;
"select"	return SELECT;
"show"		return SHOW;
"table"		return TABLE;
"tables"	return TABLES;
"use"		return USE;
"values"	return VALUES;
"varchar"	return VARCHAR;

	/* identifiers */
{alpha}+	{
				yylval.id = strdup(yytext);
				return IDENTIFIER;
			}

	/* integers */
{num}+		{
				yylval.integer = atoi(yytext);
				return INTEGER;
			}

	/* strings */
{string}	{
				yylval.string = strdup(yytext);
				//printf("string: %s[%lu]\n", yytext, yyleng);
				return STRING;
			}

"`"			return '`';
","			return ',';
"("			return '(';
")"			return ')';

";"			return *yytext;

[ \t\n]		{} /* skip whitespace */

.			yyerror("invalid character");

%%

int yywrap(void) {
	return 1;
}