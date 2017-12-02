#include<setjmp.h>

#ifndef _ERROR_H
#define PANIC(X) panic(__FILE__,__func__,__LINE__,X)
#define ERROR(X) error(__FILE__,__func__,__LINE__,X)
void panic(const char* file,const char* func,int line,const char* mes);
void error(const char* file,const char* func,int line,const char* mes);
extern jmp_buf errorreturn;
#define _ERROR_H 1
#endif
