#ifndef _CHARLIST_H

typedef struct _string{
	char c;
	struct _string* next;
	struct _string* prev;
} charlist;

charlist* mkchar(charlist* prev);
charlist* mkstr(char* str);
charlist* lastchar(charlist* s);
void addstr(charlist* s,char* str);
void insertlast(charlist* s,char c);
void addchar(charlist* s,char c);
void insertchar(charlist* s,char c);
int charlistlength(charlist* s);
void charlisttostr(charlist* s,char* d,int num);
void delcharlist(charlist* s);
#define _CHARLIST_H 1
#endif
