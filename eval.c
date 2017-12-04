#include<stdlib.h>
#include<string.h>
#include"variable.h"
#include"error.h"

variable callfunc(variable func,variable args);

void replacesymbol(variable body,variable i,variable k){

	if(body.type==TYPE_CONS){
		if(i.type==TYPE_SYM){
			if( eqvariable(((cons*)body.var)->car,i) ){
				delvariable(((cons*)body.var)->car);
				((cons*)body.var)->car=copyvariable(k);
			}
		}else{
			ERROR("置換対象シンボルとしてシンボル以外のものを指定されました。");
		}
		replacesymbol(((cons*)body.var)->car,i,k);
		replacesymbol(((cons*)body.var)->cdr,i,k);
	}
}

void openargs(variable body,variable sym,variable args){
	if(sym.type==TYPE_CONS){
		if(args.type==TYPE_CONS){
			if(((cons*)sym.var)->car.type==TYPE_SYM){
				replacesymbol(body,((cons*)sym.var)->car,((cons*)args.var)->car);
				openargs(body,((cons*)sym.var)->cdr,((cons*)args.var)->cdr);
			}else{
				ERROR("symにシンボル以外のものが指定されました。");
			}
		}else if(args.type==TYPE_NULL){
			ERROR("引数が足りません");
		}else{
			ERROR("argsにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
		}
	}else if(sym.type==TYPE_NULL){
		return ;
	}else{
		ERROR("symにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
	}
}

variable callfunc(variable f,variable args){
	variable v;
	variable head;

	v=(variable){TYPE_NULL,0};
	if(args.type==TYPE_CONS || args.type==TYPE_NULL){
		if(f.type==TYPE_IFUNC || f.type==TYPE_SPFORM){
			v=((ifunc*)f.var)->func(args);
		}else if(f.type==TYPE_LAMBDA){
			for(head=(((lambda*)f.var)->body);head.type==TYPE_CONS;head=(((cons*)head.var)->cdr) ){
				openargs(((cons*)head.var)->car,((lambda*)f.var)->args,args);
				delvariable(v);
				v=eval(((cons*)head.var)->car);
			}
		}else{
			ERROR("関数でないものを実行しようとしました。");
		}
	}else if(args.type!=TYPE_NULL){
		ERROR("argsにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
		fprintf(stderr,"NOTE:渡された引数={type=%d,var=%p}\n",args.type,args.var);
	}
	return v;
}

variable eval(variable c){
	variable rhead,whead;
	variable r,res;

	if(c.type==TYPE_CONS){
		r=newvariable(TYPE_CONS,0);
		whead=r;
		((cons*)r.var)->car=eval(((cons*)c.var)->car);
		if( ((cons*)r.var)->car.type == TYPE_IFUNC || ((cons*)r.var)->car.type == TYPE_LAMBDA){
			for(rhead=((cons*)c.var)->cdr;rhead.type==TYPE_CONS;rhead=((cons*)rhead.var)->cdr){
				((cons*)whead.var)->cdr=newvariable(TYPE_CONS,0);
				whead=((cons*)whead.var)->cdr;
				((cons*)whead.var)->car=eval(((cons*)rhead.var)->car);
			}
			((cons*)whead.var)->cdr=(variable){TYPE_NULL,0};
			res=callfunc(((cons*)r.var)->car,((cons*)r.var)->cdr);
			delvariable(r);
			return res;
		}else if( ((cons*)r.var)->car.type == TYPE_SPFORM ){
			((cons*)r.var)->cdr=copyvariable(((cons*)c.var)->cdr);
			res = callfunc(((cons*)r.var)->car,((cons*)r.var)->cdr);
			return res;
		}else{
			ERROR("関数でないものを実行しようとしました。");
			delvariable(r);
			return (variable){TYPE_NULL,0};
		}
	}else if(c.type==TYPE_SYM){
		return get((char*)c.var,symbols);
	}else{
		return copyvariable(c);
	}
}
