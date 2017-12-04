#include<stdio.h>
#include<stdlib.h>
#include"charlist.h"
#include"variable.h"
#include"error.h"

char* readvar(char* str);
variable readsymbol(char* str);
variable readnum(char* str);
variable readcons(char* str);

char* readvar(char* str){
	char* ptr;
	charlist* s;
	char* r;

	s=mkchar(0);
	for(ptr=str;*ptr!=' ' && *ptr!=')' && *ptr!='(' && *ptr!='.' && *ptr!='\0';ptr++){
		insertlast(s,*ptr);
	}
	r=(char*)malloc(sizeof(char)*charlistlength(s));
	charlisttostr(s,r,sizeof(char)*charlistlength(s));
	delcharlist(s);
	return r;
}

variable readsymbol(char* str){
	return (variable){TYPE_SYM,readvar(str)};
}

variable readnum(char* str){
	char* dst;
	double* i;
	dst=readvar(str);
	i=(double*)malloc(sizeof(double));
	*i=0;
	*i=atof(dst);
	free(dst);
	return (variable){TYPE_NUM,i};
}

variable readstr(char* str){
	char* ptr;
	charlist* s;
	variable v;
	char term=*str;

	s=mkchar(0);
	for(ptr=str+1;*ptr!=term;ptr++){
		if(*ptr==term){
			ptr++;
			insertlast(s,*ptr);
		}else if(*ptr=='\0'){
			ERROR("文字列の途中で入力が終了しました。");
		}else{
			insertlast(s,*ptr);
		}
	}
	v=newvariable(TYPE_STR,charlistlength(s));
	charlisttostr(s,((string*)v.var)->str,((string*)v.var)->size);
	delcharlist(s);
	return v;
}

variable readcons(char* str){
	char* ptr;
	int braces=0;
	cons *c,*h;
	cons *p;
	int excar=0,refcar=1,excdr=0;;

	p=c=h=0;
	
	for(ptr=str;*ptr;ptr++){
		switch(*ptr){
		case '(':
			braces++;
			if(braces==2){
				if(refcar){
					if(excar){
						h->cdr=(variable){TYPE_CONS,mkcons()};
						excdr=1;
						h=h->cdr.var;
						excar=0;
						excdr=0;
						h->car=readcons(ptr);
						excar=1;
					}else{
						h->car=readcons(ptr);
						excar=1;
					}
				}else{
					if(excdr){
						ERROR("CDRはすでに代入済みです。");
					}else{
						h->cdr=readcons(ptr);
						excdr=1;
					}
				}
			}else if(braces==1){
				h=c=mkcons();
				excar=0;
			}
			break;
		case ')':
			braces--;
			if(braces==0){
				return (variable){TYPE_CONS,(void*)c};
			}
			break;
		case '.':
			refcar=0;
			break;
		default:
			if(braces==1){
				if('0'<=*ptr && *ptr<='9'){
					if(refcar){
						if(excar){
							h->cdr=(variable){TYPE_CONS,mkcons()};
							excdr=1;
							h=h->cdr.var;
							excar=0;
							excdr=0;
							h->car=readnum(ptr);
							excar=1;
						}else{
							h->car=readnum(ptr);
							excar=1;
						}
					}else{
						if(excdr){
							ERROR("CDRはすでに代入済みです。");
						}else{
							h->cdr=readnum(ptr);
							excdr=1;
						}
					}
					while(*ptr!=' ' && *ptr!=')' && *ptr!='(' && *ptr!='.' )ptr++;
					ptr--;
				}else if(*ptr=='\"'){
					if(refcar){
						if(excar){
							h->cdr=(variable){TYPE_CONS,mkcons()};
							excdr=1;
							h=h->cdr.var;
							excar=0;
							excdr=0;
							h->car=readstr(ptr);
							excar=1;
						}else{
							h->car=readstr(ptr);
							excar=1;
						}
					}else{
						if(excdr){
							ERROR("CDRはすでに代入済みです。");
						}else{
							h->cdr=readstr(ptr);
							excdr=1;
						}
					}
					ptr++;
					while(*ptr!='\"')ptr++;
				}else if(*ptr!=' '){
					if(refcar){
						if(excar){
							h->cdr=(variable){TYPE_CONS,mkcons()};
							excdr=1;
							h=h->cdr.var;
							excar=0;
							excdr=0;
							h->car=readsymbol(ptr);
							excar=1;
						}else{
							h->car=readsymbol(ptr);
							excar=1;
						}
					}else{
						if(excdr){
							ERROR("CDRはすでに代入済みです。");
						}else{
							h->cdr=readsymbol(ptr);
							excdr=1;
						}
					}
					while(*ptr!=' ' && *ptr!=')' && *ptr!='(' && *ptr!='.' )ptr++;
					ptr--;
				}
			}
			break;
		}
	}
	ERROR("括弧が閉じていません。");
	return (variable){TYPE_NULL,(void*)0};
}

variable read(char* str){
	char* ptr;
	for(ptr=str;*ptr==' ' && *ptr!='\0';ptr++);
	if(*ptr=='('){
		return readcons(ptr);
	}else if('0'<=*ptr && *ptr<='9'){
		return readnum(ptr);
	}else if(*ptr=='\"'){
		return readstr(ptr);
	}else if(*ptr=='\0'){
		return (variable){TYPE_NULL,0};
	}else{
		return readsymbol(ptr);
	}
}

