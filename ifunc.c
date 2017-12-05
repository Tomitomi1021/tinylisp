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

variable _plus(variable args,struct symbolstack* st){
	return newnum(plus(args));
}
variable _minus(variable args,struct symbolstack* st){
	return newnum(minus(args));
}
variable _multi(variable args,struct symbolstack* st){
	return newnum(multi(args));
}
variable _divs(variable args,struct symbolstack* st){
	return newnum(divs(args));
}

variable quit(variable args,struct symbolstack* st){
	exit(0);
}

variable atom(variable args,struct symbolstack* st){
	if(args.type==TYPE_CONS){
		if(((cons*)args.var)->car.type==TYPE_CONS){
			return newvariable(TYPE_NULL,0);
		}else{
			return newvariable(TYPE_T,0);
		}
	}else{
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
}

variable eq(variable args,struct symbolstack* st){
	variable A,B;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;
	if(eqvariable(A,B)){
		return newvariable(TYPE_T,0);
	}else{
		return newvariable(TYPE_NULL,0);
	}
}

variable car(variable args,struct symbolstack* st){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->car.type!=TYPE_CONS){
		ERROR("引数がTYPE_CONSではありません");
		return newvariable(TYPE_NULL,0);
	}
	return copyvariable(((cons*)((cons*)args.var)->car.var)->car);
}

variable cdr(variable args,struct symbolstack* st){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->car.type!=TYPE_CONS){
		ERROR("引数がTYPE_CONSではありません");
		return newvariable(TYPE_NULL,0);
	}
	return copyvariable(((cons*)((cons*)args.var)->car.var)->cdr);
}

variable _cons(variable args,struct symbolstack* st){
	variable A,B;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;
	return newcons(A,B);
}

variable quote(variable args,struct symbolstack* st){
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}else{
		return copyvariable( ((cons*)args.var)->car );
	}
}

variable define(variable args,struct symbolstack* st){
	variable A,B,C;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;
	C=eval(B,st);
	sset((char*)A.var,C,st);
	return C;
}

variable _if(variable args,struct symbolstack* st){
	variable A,B,C;

	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	
	A=((cons*)args.var)->car;
	B=((cons*)(((cons*)args.var)->cdr.var))->car;

	if( ((cons*)(((cons*)args.var)->cdr.var))->cdr.type==TYPE_CONS ){
		C=((cons*)(((cons*)(((cons*)args.var)->cdr.var))->cdr.var))->car;
	}else{
		C=newvariable(TYPE_NULL,0);
	}

	if( eval(A,st).type == TYPE_NULL){
		return eval(C,st);
	}else{
		return eval(B,st);
	}
}

variable _lambda(variable args,struct symbolstack* st){
	variable A,B;
	variable R;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->cdr.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	if(((cons*)args.var)->car.type!=TYPE_CONS){
		ERROR("引数がTYPE_CONSではありません");
		return newvariable(TYPE_NULL,0);
	}
	A=((cons*)args.var)->car;
	R=newvariable(TYPE_LAMBDA,0);
	((lambda*)R.var)->args=A;
	((lambda*)R.var)->body=((cons*)args.var)->cdr;
	return R;
}

variable _import(variable args,struct symbolstack* st){
	variable A;
	FILE* fp;
	if(args.type==TYPE_NULL){
		ERROR("引数が足りません");
		return newvariable(TYPE_NULL,0);
	}
	A=((cons*)args.var)->car;
	if(A.type!=TYPE_STR){
		ERROR("引数がTYPE_STRではありません");
		return newvariable(TYPE_NULL,0);
	}
	fp=fopen(((string*)A.var)->str,"r");
	if(fp==0){
		ERROR("ファイルのオープンに失敗しました");
		return newvariable(TYPE_NULL,0);
	}
	if(repl(fp,0)==0){
		fclose(fp);
		return newvariable(TYPE_T,0);
	}
	fclose(fp);
	return newvariable(TYPE_NULL,0);
}

void initifunc(){
	variable f;
	f=newifunc(TYPE_NUM,_plus);set("+",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_minus);set("-",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_multi);set("*",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_divs);set("/",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,quit);set("quit",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,atom);set("atom",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,eq);set("eq",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,car);set("car",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,cdr);set("cdr",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_cons);set("cons",f,symbols.symbols);delvariable(f);
	f=newifunc(TYPE_NUM,_import);set("import",f,symbols.symbols);delvariable(f);

	f=newspform(TYPE_NUM,quote);set("quote",f,symbols.symbols);delvariable(f);
	f=newspform(TYPE_NUM,define);set("define",f,symbols.symbols);delvariable(f);
	f=newspform(TYPE_NUM,_if);set("if",f,symbols.symbols);delvariable(f);
	f=newspform(TYPE_NUM,_lambda);set("lambda",f,symbols.symbols);delvariable(f);
}
