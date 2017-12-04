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

typedef struct{
	int type;
	variable (*func)(variable);
} ifunc;

typedef struct{
	int type;
	variable args;
	variable body;
} lambda;

struct symboltable{
	char exist;
	char symbol[SYMBOLSIZE];
	variable var;
};

extern struct symboltable symbols[SYMBOLTABLESIZE];
extern struct symboltable runtimesymbols[SYMBOLTABLESIZE];

cons* mkcons();

variable newvariable(int type,int option);
variable newcons(variable car,variable cdr);
//variable newsym(char* name);
variable newnum(double i);
variable newifunc(int type,variable (*func)(variable));
variable newspform(int type,variable (*func)(variable));
variable copyvariable(variable v);
int eqvariable(variable A,variable B);
void delvariable(variable v);
double varnum(variable v);

variable read(char* str);

void print(variable var,FILE* fp);

variable eval(variable c);

void set(char* symbol,variable v,struct symboltable* table);
variable get(char* symbol,struct symboltable* table);
void allclear(struct symboltable* table);
void initsymboltable();
void initifunc();
#define _VARIABLE_H 1
#endif
