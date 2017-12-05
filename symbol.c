#include<stdlib.h>
#include<string.h>
#include"variable.h"
#include"error.h"


struct symbolstack symbols;


void symbolstackdump(struct symbolstack* st,FILE* fp){
	if(st){
		fprintf(fp,"======================\n");
		symboldump(st->symbols,fp);
		fprintf(fp,"======================\n");
		symbolstackdump(st->prev,fp);
	}
}

void symboldump(struct symboltable* symbols,FILE* fp){
	int A;
	for(A=0;A<SYMBOLTABLESIZE;A++){
		if(symbols[A].exist){
			fprintf(fp,"%s:",symbols[A].symbol);
			print(symbols[A].var,fp);
			fprintf(fp,"\n");
		}
	}
}

void sset(char* symbol,variable v,struct symbolstack* st){
	set(symbol,v,st->symbols);
}

variable sget(char* symbol,struct symbolstack* st){
	if(symbolexist(symbol,st->symbols)){
		return get(symbol,st->symbols);
	}else if(st->prev!=0){
		return sget(symbol,st->prev);
	}else{
		fprintf(stderr,"シンボル名:%s\n",symbol);
		ERROR("シンボルに対応するエントリがありません。");
		return newvariable(TYPE_NULL,0);
	}
}

struct symbolstack* newsymbolstack(struct symbolstack* prev){
	struct symbolstack* st;
	int A;
	st=(struct symbolstack*)malloc(sizeof(struct symbolstack));
	if(st==0){
		PANIC("メモリーの確保に失敗しました。");
	}
	st->prev=prev;
	for(A=0;A<SYMBOLTABLESIZE;A++){
		st->symbols[A].exist=0;
	}
	return st;
}

void delsymbolstack(struct symbolstack* st){
	allclear(st->symbols);
	free(st);
}

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

int symbolexist(char* symbol,struct symboltable* table){
	int A;
	for(A=0;A<SYMBOLTABLESIZE;A++){
		if(table[A].exist && !strcmp(symbol,table[A].symbol))return 1;
	}
	return 0;
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
		symbols.symbols[i].exist=0;
	}
	symbols.prev=0;
	set("NIL",(variable){TYPE_NULL,0},symbols.symbols);
	set("T",(variable){TYPE_T,0},symbols.symbols);
}
