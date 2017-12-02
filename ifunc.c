#include<stdlib.h>
#include<string.h>
#include"variable.h"
#include"error.h"

variable callfunc(char* func,variable args);


double plus(variable args){
	if(args.type==TYPE_NULL){
		return 0;
	}else{
		return plus(((cons*)args.var)->cdr)+varnum(((cons*)args.var)->car);
	}
}

double minus(variable args){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return 0;
	}else{
		return varnum(((cons*)args.var)->car)-plus(((cons*)args.var)->cdr);
	}
}

double multi(variable args){
	if(args.type==TYPE_NULL){
		return 1;
	}else{
		return multi(((cons*)args.var)->cdr)*varnum(((cons*)args.var)->car);
	}
}

double divs(variable args){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return 0;
	}else{
		return varnum(((cons*)args.var)->car)/multi(((cons*)args.var)->cdr);
	}
}

variable _plus(variable args){
	return newnum(plus(args));
}
variable _minus(variable args){
	return newnum(minus(args));
}
variable _multi(variable args){
	return newnum(multi(args));
}
variable _divs(variable args){
	return newnum(divs(args));
}

variable quit(variable args){
	exit(0);
}

variable atom(variable args){
	if(args.type==TYPE_CONS){
		if(((cons*)args.var)->car.type==TYPE_CONS){
			return newvariable(TYPE_NULL);
		}else{
			return newvariable(TYPE_T);
		}
	}else{
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
}

variable eq(variable args){
	variable A,B;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;
	if(eqvariable(A,B)){
		return newvariable(TYPE_T);
	}else{
		return newvariable(TYPE_NULL);
	}
}

variable car(variable args){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->car.type!=TYPE_CONS){
		ERROR("引数がTYPE_CONSではありません");
		return newvariable(TYPE_NULL);
	}
	return copyvariable(((cons*)((cons*)args.var)->car.var)->car);
}

variable cdr(variable args){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->car.type!=TYPE_CONS){
		ERROR("引数がTYPE_CONSではありません");
		return newvariable(TYPE_NULL);
	}
	return copyvariable(((cons*)((cons*)args.var)->car.var)->cdr);
}

variable _cons(variable args){
	variable A,B;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;
	return newcons(A,B);
}

variable quote(variable args){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}else{
		return copyvariable( ((cons*)args.var)->car );
	}
}

variable define(variable args){
	variable A,B,C;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;
	C=eval(B);
	set((char*)A.var,C,symbols);
	return C;
}

variable _if(variable args){
	variable A,B,C;

	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;

	if( ((cons*)(((cons*)args.var)->cdr.var))->cdr.type==TYPE_CONS ){
		C=((cons*)(((cons*)(((cons*)args.var)->cdr.var))->cdr.var))->car;
	}else{
		C=newvariable(TYPE_NULL);
	}

	if( eval(A).type == TYPE_NULL){
		return eval(C);
	}else{
		return eval(B);
	}
}

variable _lambda(variable args){
	variable A,B;
	variable R;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL);
	}
	if(((cons*)args.var)->car.type!=TYPE_CONS){
		ERROR("引数がTYPE_CONSではありません");
		return newvariable(TYPE_NULL);
	}
	A=((cons*)args.var)->car;
	R=newvariable(TYPE_LAMBDA);
	((lambda*)R.var)->args=A;
	((lambda*)R.var)->body=((cons*)args.var)->cdr;
	return R;
}

void initifunc(){
	variable f;
	f=newifunc(TYPE_NUM,_plus);set("+",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_minus);set("-",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_multi);set("*",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_divs);set("/",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,quit);set("quit",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,atom);set("atom",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,eq);set("eq",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,car);set("car",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,cdr);set("cdr",f,symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_cons);set("cons",f,symbols);delvariable(f);

	f=newspform(TYPE_NUM,quote);set("quote",f,symbols);delvariable(f);
	f=newspform(TYPE_NUM,define);set("define",f,symbols);delvariable(f);
	f=newspform(TYPE_NUM,_if);set("if",f,symbols);delvariable(f);
	f=newspform(TYPE_NUM,_lambda);set("lambda",f,symbols);delvariable(f);
}
