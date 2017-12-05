#include<stdio.h>

#ifndef _VARIABLE_H
#define TYPE_NULL	0
#define TYPE_T		1
#define TYPE_CONS	2
#define TYPE_SYM	3
#define TYPE_NUM	4
#define TYPE_STR	5
#define TYPE_LAMBDA	6
#define TYPE_IFUNC	7
#define TYPE_SPFORM	8

#define SYMBOLSIZE 32
#define SYMBOLTABLESIZE 256


typedef struct{
	int type;
	void* var;
} variable;

typedef struct{
	variable car;
	variable cdr;
} cons;

typedef struct{
	int size;
	char* str;
} string;


struct symboltable{
	char exist;
	char symbol[SYMBOLSIZE];
	variable var;
};

struct symbolstack{
	struct symboltable symbols[SYMBOLTABLESIZE];
	struct symbolstack *prev;
};

typedef struct{
	int type;
	variable (*func)(variable,struct symbolstack*);
} ifunc;

typedef struct{
	int type;
	variable args;
	variable body;
} lambda;

extern struct symbolstack symbols;

cons* mkcons();

variable newvariable(int type,int option);
variable newcons(variable car,variable cdr);
//variable newsym(char* name);
variable newnum(double i);
variable newifunc(int type,variable (*func)(variable,struct symbolstack*));
variable newspform(int type,variable (*func)(variable,struct symbolstack*));
variable copyvariable(variable v);
int eqvariable(variable A,variable B);
void delvariable(variable v);
double varnum(variable v);

variable read(char* str);

void print(variable var,FILE* fp);

variable eval(variable c,struct symbolstack* st);

void symbolstackdump(struct symbolstack* st,FILE* fp);
struct symbolstack* newsymbolstack(struct symbolstack* prev);
void delsymbolstack(struct symbolstack* st);

void sset(char* symbol,variable v,struct symbolstack* st);
variable sget(char* symbol,struct symbolstack* st);

void symboldump(struct symboltable* symbols,FILE* fp);
void set(char* symbol,variable v,struct symboltable* table);
variable get(char* symbol,struct symboltable* table);
int symbolexist(char* symbol,struct symboltable *table);
void allclear(struct symboltable* table);
void initsymboltable();
void initifunc();

int repl(FILE* fpin,FILE* fpout);
#define _VARIABLE_H 1
#endif
