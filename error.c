#include<stdlib.h>
#include"error.h"
#include<stdio.h>
#include<setjmp.h>

jmp_buf errorreturn;

void panic(const char* file,const char* func,int line,const char* mes){
	fprintf(stderr,"\n致命的なエラー\n");
	fprintf(stderr,"ファイル名:%s\n",file);
	fprintf(stderr,"関数名:%s\n",func);
	fprintf(stderr,"行:%d\n",line);
	fprintf(stderr,"%s\n",mes);
	exit(1);
}

void error(const char* file,const char* func,int line,const char* mes){
	fprintf(stderr,"\nエラー\n");
	fprintf(stderr,"ファイル名:%s\n",file);
	fprintf(stderr,"関数名:%s\n",func);
	fprintf(stderr,"行:%d\n",line);
	fprintf(stderr,"%s\n",mes);
	longjmp(errorreturn,1);
}
