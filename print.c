#include<stdio.h>
#include"variable.h"
#include"error.h"

void printcons(cons* c,FILE* fp);

void printcons(cons* c,FILE* fp){
	print(c->car,fp);
	if(c->cdr.type==TYPE_CONS){
		fputc(' ',fp);
		printcons(c->cdr.var,fp);
	}else if(c->cdr.type!=TYPE_NULL){
		fputc('.',fp);
		print(c->cdr,fp);
	}
}

void print(variable var,FILE* fp){
	switch(var.type){
	case TYPE_CONS:
		fputc('(',fp);
		printcons((cons*)var.var,fp);
		fputc(')',fp);
		break;
	case TYPE_NULL:
		fputs("NIL",fp);
		break;
	case TYPE_SYM:
		fprintf(fp,"%s",(char*)var.var);
		break;
	case TYPE_NUM:
		if( (*((double*)var.var)-(int)*((double*)var.var)) == 0){
			fprintf(fp,"%d",(int)*((double*)var.var));
		}else{
			fprintf(fp,"%lf",*((double*)var.var));
		}
		break;
	case TYPE_T:
		fputs("T",fp);
		break;
	case TYPE_IFUNC:
		fprintf(fp,"<組み込み関数 %p>",((ifunc*)var.var)->func);
		break;
	case TYPE_SPFORM:
		fprintf(fp,"<特別形式 %p>",((ifunc*)var.var)->func);
		break;
	case TYPE_LAMBDA:
		if(((lambda*)var.var)->body.type==TYPE_CONS){
			fprintf(fp,"<LAMBDA ");
			print(((lambda*)var.var)->args,fp);
			fputc(' ',fp);
			printcons((cons*)((lambda*)var.var)->body.var,fp);
			fprintf(fp,">");
		}else{
			ERROR("lambda構造体のbodyの値が不正です。");
		}
		break;
	case TYPE_STR:
		fprintf(fp,"\"%s\"",((string*)var.var)->str);
		break;
	default:
		ERROR("定義されていないタイプのデータを表示しようとしました。");
		fprintf(stderr,"タイプ:%d\n",var.type);
	}
}
