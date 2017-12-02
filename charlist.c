#include<stdlib.h>
#include<string.h>
#include"charlist.h"

charlist* mkchar(charlist* prev){
	charlist *s;
	int i;
	s=(charlist*)malloc(sizeof(charlist));
	s->next=0;
	s->prev=prev;
	s->c=0;
	return s;
}

charlist* mkstr(char* str){
	charlist *s;
	s=mkchar(0);
	addstr(s,str);
	return s;
}

charlist* lastchar(charlist* s){
	if(s->next){
		return lastchar(s->next);
	}else{
		return s;
	}
}

void addstr(charlist* s,char* str){
	char* ptr;
	charlist* h=s;

	for(ptr=str;*ptr;ptr++){
		insertlast(h,*ptr);
		if(h->next)h=h->next;
	}
}

void insertlast(charlist* s,char c){
	insertchar(lastchar(s),c);
}

void addchar(charlist* s,char c){
	charlist *l,*k;
//	s l k
	k=s->next;

	l=s->next=mkchar(s);
	l->c=c;
	l->prev=s;
	l->next=k;
	if(k){
		k->prev=l;
	}
}

void insertchar(charlist* s,char c){
	charlist *k;

	if(s->prev){
		addchar(s->prev,c);
	}else{
		addchar(s,s->c);
		s->c=c;
	}
}

int charlistlength(charlist* s){
	if(s->next){
		return charlistlength(s->next)+1;
	}else{
		return 1;
	}
}

void charlisttostr(charlist* s,char* d,int num){
	if(s->next && num>1){
		*d=s->c;
		charlisttostr(s->next,d+1,num-1);
	}else{
		*d=0;
	}
}

void delcharlist(charlist* s){
	if(s->next){
		delcharlist(s->next);
	}
	free(s);
}
