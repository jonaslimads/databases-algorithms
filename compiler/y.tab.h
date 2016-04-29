/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     IDENTIFIER = 259,
     STRING = 260,
     CREATE = 261,
     DATABASE = 262,
     DELETE = 263,
     DESCRIBE = 264,
     EXIT = 265,
     FROM = 266,
     INSERT = 267,
     INT = 268,
     INTO = 269,
     QUIT = 270,
     SELECT = 271,
     SHOW = 272,
     TABLE = 273,
     TABLES = 274,
     USE = 275,
     VALUES = 276,
     VARCHAR = 277
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define IDENTIFIER 259
#define STRING 260
#define CREATE 261
#define DATABASE 262
#define DELETE 263
#define DESCRIBE 264
#define EXIT 265
#define FROM 266
#define INSERT 267
#define INT 268
#define INTO 269
#define QUIT 270
#define SELECT 271
#define SHOW 272
#define TABLE 273
#define TABLES 274
#define USE 275
#define VALUES 276
#define VARCHAR 277




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 32 "dbms.y"
{
	int integer;
	char *id;
	char *string;
	struct terminal *t;
	struct tableColumnDefinition *table_column_definition;
}
/* Line 1529 of yacc.c.  */
#line 101 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

