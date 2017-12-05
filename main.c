#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"charlist.h"
#include"variable.h"

#include<signal.h>
#include<setjmp.h>
#include"error.h"


int repl(FILE* fpin,FILE* fpout){
	charlist* str;
	char* s;
	int c;
	variable v,v2;
	int brace=0;

	if(!fpin)ERROR("入力がありません");

	str=mkchar(0);

	if(fpout)fprintf(fpout,">>");
	while(1){
		c=fgetc(fpin);
		if(feof(fpin)){
			if(brace==0){
				return 0;
			}else{
				ERROR("式の途中で入力が終了しました。");
				return 1;
			}
		}
		if(c=='(')brace++;
		if(c==')')brace--;
		if(c=='\n'){
			if(brace==0){
				c=charlistlength(str);
				s=(char*)malloc(sizeof(char)*c);
				charlisttostr(str,s,c);
				delcharlist(str);
				str=mkchar(0);
				v=read(s);
				v2=eval(v,&symbols);
				if(fpout)print(v2,fpout);
				if(fpout)fputc('\n',fpout);
				if(fpout)fprintf(fpout,">>");
				delvariable(v);
				delvariable(v2);
				free(s);
			}else{
				insertlast(str,' ');
			}
		}else if(c=='\t'){
			insertlast(str,' ');
		}else{
			insertlast(str,c);
		}
	}
	
}

void constest(){
	char str[256];
	char *ptr;
	variable c;

	for(ptr=str;1;ptr++){
		*ptr=fgetc(stdin);
		if(*ptr=='\n'){
			*ptr='\0';
			break;
		}
	}

	c=read(str);
	print(c,stdout);
}

void strtest(){
	charlist* s;
	char str[256];

	s=mkstr("This is");
	insertlast(s,'a');
	addstr(s,"pen.");
	charlisttostr(s,str,256);
	printf("%s\n%d\n",str,charlistlength(s));
	delcharlist(s);
}


int main(){
	FILE* fp;

	initsymboltable();
	initifunc();
	printf("Welcome to TINY LISP\n");
	setjmp(errorreturn);
	repl(stdin,stdout);
	return 0;
}
