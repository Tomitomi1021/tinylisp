#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"charlist.h"
#include"variable.h"
#include"error.h"

cons* mkcons(){
	variable c;
	c=newvariable(TYPE_CONS);
	return (cons*)c.var;
}

double varnum(variable v){
	if(v.type==TYPE_NUM){
		return *((double*)v.var);
	}else{
		ERROR("引数としてTYPE_NUMでないデータが渡されました。");
		fprintf(stderr,"タイプ:%d\n",v.type);
		return 0;
	}
}

int eqvariable(variable A,variable B){
	if(A.type!=B.type){
		return 0;
	}else{
		switch(A.type){
			case TYPE_SYM:
				return !strcmp((char*)A.var,(char*)B.var);
			case TYPE_NUM:
				return *((double*)A.var)==*((double*)B.var);
			case TYPE_CONS:
				return 0;
			case TYPE_T:
				return 1;
			case TYPE_NULL:
				return 1;
			default:
				ERROR("定義されていない比較が行われました。");
				return 0;
				break;
		}
	}
}

variable copyvariable(variable v){
	variable r;

	r=newvariable(v.type);
	switch(v.type){
	case TYPE_CONS:
		((cons*)r.var)->car=copyvariable(((cons*)v.var)->car);
		((cons*)r.var)->cdr=copyvariable(((cons*)v.var)->cdr);
		break;
	case TYPE_SYM:
		strcpy(r.var,v.var);
		break;
	case TYPE_NUM:
		*((double*)r.var)=*((double*)v.var);
		break;
	case TYPE_IFUNC:
		((ifunc*)r.var)->func=((ifunc*)v.var)->func;
		((ifunc*)r.var)->type=((ifunc*)v.var)->type;
		break;
	case TYPE_T:
	case TYPE_NULL:
		break;
	case TYPE_SPFORM:
		((ifunc*)r.var)->func=((ifunc*)v.var)->func;
		((ifunc*)r.var)->type=((ifunc*)v.var)->type;
		break;
	case TYPE_LAMBDA:
		((lambda*)r.var)->body = copyvariable(((lambda*)v.var)->body);
		((lambda*)r.var)->args = copyvariable(((lambda*)v.var)->args);
		break;
	default:
		ERROR("定義されていないタイプのデータをコピーしようとしました。");
		fprintf(stderr,"タイプ:%d\n",v.type);
	}
	return r;
}


void delvariable(variable v){
	switch(v.type){
	case TYPE_CONS:
		delvariable(((cons*)v.var)->car);
		delvariable(((cons*)v.var)->cdr);
		free(v.var);
		break;
	case TYPE_LAMBDA:
		delvariable(((lambda*)v.var)->args);
		delvariable(((lambda*)v.var)->body);
		free(v.var);
		break;
	case TYPE_IFUNC:
	case TYPE_SPFORM:
	case TYPE_NULL:
	case TYPE_T:
		break;
	case TYPE_NUM:
	case TYPE_SYM:
		free(v.var);
		break;
	default:
		ERROR("定義されていないタイプのデータを削除しようとしました。");
		fprintf(stderr,"タイプ:%d\n",v.type);
	}
}

variable newvariable(int type){
	variable v;

	v.type=type;
	switch(type){
	case TYPE_NULL:
		v.var=0;
		break;
	case TYPE_CONS:
		v.var=malloc(sizeof(cons));
		if(v.var==0)PANIC("メモリーの確保に失敗しました。");
		((cons*)v.var)->car=newvariable(TYPE_NULL);
		((cons*)v.var)->cdr=newvariable(TYPE_NULL);
		break;
	case TYPE_SYM:
		v.var=malloc(sizeof(char)*SYMBOLSIZE);
		if(v.var==0)PANIC("メモリーの確保に失敗しました。");
		break;
	case TYPE_NUM:
		v.var=malloc(sizeof(double));
		if(v.var==0)PANIC("メモリーの確保に失敗しました。");
		*((double*)v.var)=0;
		break;
	case TYPE_LAMBDA:
		v.var=malloc(sizeof(lambda));
		if(v.var==0)PANIC("メモリーの確保に失敗しました。");
		((lambda*)v.var)->args=newvariable(TYPE_NULL);
		((lambda*)v.var)->body=newvariable(TYPE_NULL);
		((lambda*)v.var)->type=TYPE_NULL;
		break;
	case TYPE_IFUNC:
		v.var=malloc(sizeof(ifunc));
		if(v.var==0)PANIC("メモリーの確保に失敗しました。");
		((ifunc*)v.var)->type=TYPE_NULL;
		break;
	case TYPE_T:
		v.var=0;
		break;
	case TYPE_SPFORM:
		v.var=malloc(sizeof(ifunc));
		if(v.var==0)PANIC("メモリーの確保に失敗しました。");
		((ifunc*)v.var)->type=TYPE_NULL;
		break;
	default:
		ERROR("定義されていないタイプのデータを生成しようとしました。");
		fprintf(stderr,"タイプ:%d\n",type);
	}
	return v;
}

variable newcons(variable car,variable cdr){
	variable v;
	v=newvariable(TYPE_CONS);
	((cons*)v.var)->car=copyvariable(car);
	((cons*)v.var)->cdr=copyvariable(cdr);
	return v;
}

variable newifunc(int type,variable (*func)(variable)){
	variable v;
	v=newvariable(TYPE_IFUNC);
	((ifunc*)v.var)->type=type;
	((ifunc*)v.var)->func=func;
	return v;
}

variable newspform(int type,variable (*func)(variable)){
	variable v;
	v=newvariable(TYPE_SPFORM);
	((ifunc*)v.var)->type=type;
	((ifunc*)v.var)->func=func;
	return v;
}

variable newnum(double i){
	variable v;
	v=newvariable(TYPE_NUM);
	*((double*)v.var)=i;
	return v;
}
