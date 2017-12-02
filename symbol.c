#include<stdlib.h>
#include<string.h>
#include"variable.h"
#include"error.h"


struct symboltable symbols[SYMBOLTABLESIZE],runtimesymbols[SYMBOLTABLESIZE];


void set(char* symbol,variable v,struct symboltable* table){
	int A;
	for(A=0;A<SYMBOLTABLESIZE;A++){
		if(table[A].exist && !strcmp(symbol,table[A].symbol))break;
	}
	if(A==SYMBOLTABLESIZE){
		for(A=0;A<SYMBOLTABLESIZE;A++){
			if(!table[A].exist)break;
		}
		if(A==SYMBOLTABLESIZE){
			PANIC("シンボルテーブルサイズが足りません");
		}else{
			table[A].exist=1;
			strncpy(table[A].symbol,symbol,SYMBOLSIZE);
			table[A].var=copyvariable(v);
		}
	}else{
		delvariable(table[A].var);
		table[A].var=copyvariable(v);
	}
}

variable get(char* symbol,struct symboltable* table){
	int A;
	for(A=0;A<SYMBOLTABLESIZE;A++){
		if(table[A].exist && !strcmp(symbol,table[A].symbol))break;
	}
	if(A==SYMBOLTABLESIZE){
		ERROR("シンボルに対応するエントリがありません。");
		fprintf(stderr,"シンボル名:%s\n",symbol);
		return (variable){TYPE_NULL,0};
	}else{
		return copyvariable(table[A].var);
	}
}

void allclear(struct symboltable* table){
	int i;
	for(i=0;i<SYMBOLTABLESIZE;i++){
		if(table[i].exist){
			delvariable(table[i].var);
			table[i].exist=0;
		}
	}
}

void initsymboltable(){
	int i;
	for(i=0;i<SYMBOLTABLESIZE;i++){
		symbols[i].exist=0;
		runtimesymbols[i].exist=0;
	}
	set("NIL",(variable){TYPE_NULL,0},symbols);
	set("T",(variable){TYPE_T,0},symbols);
}
