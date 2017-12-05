#include<stdlib.h>
#include<string.h>
#include"variable.h"
#include"error.h"

variable callfunc(variable func,variable args,struct symbolstack* st);


void openargs(struct symbolstack* st,variable sym,variable args){
	if(sym.type==TYPE_CONS){
		if(args.type==TYPE_CONS){
			if(((cons*)sym.var)->car.type == TYPE_SYM){
				if(!strcmp( ((cons*)sym.var)->car.var,"&rest")){
					if(((cons*)sym.var)->cdr.type==TYPE_CONS){
						sset( ((cons*)(((cons*)sym.var)->cdr.var))->car.var,args,st);
					}else{
						ERROR("symにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
					}
				}else{
					sset( ((cons*)sym.var)->car.var , ((cons*)(args.var))->car , st);
					openargs(st,((cons*)sym.var)->cdr,((cons*)(args.var))->cdr);
				}
			}else{
				ERROR("symにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
			}
		}else if(args.type==TYPE_NULL){
			if(!strcmp( ((cons*)sym.var)->car.var,"&rest")){
				if(((cons*)sym.var)->cdr.type==TYPE_CONS){
					sset( ((cons*)(((cons*)sym.var)->cdr.var))->car.var,args,st);
				}else{
					ERROR("symにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
				}
			}else{
				ERROR("引数が足りません");
			}
		}else{
			ERROR("argsにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
		}
	}else if(sym.type==TYPE_NULL){
		return ;
	}else{
		ERROR("symにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
	}
}

variable callfunc(variable f,variable args,struct symbolstack* st){
	variable v;
	variable head;
	struct symbolstack* st2;

	v=(variable){TYPE_NULL,0};
	if(args.type==TYPE_CONS || args.type==TYPE_NULL){
		if(f.type==TYPE_IFUNC || f.type==TYPE_SPFORM){
			v=((ifunc*)f.var)->func(args,st);
		}else if(f.type==TYPE_LAMBDA){
			st2=newsymbolstack(st);
			openargs(st2,((lambda*)f.var)->args,args);
			for(head=(((lambda*)f.var)->body);head.type==TYPE_CONS;head=(((cons*)head.var)->cdr) ){
				delvariable(v);
				v=eval(((cons*)head.var)->car,st2);
			}
			delsymbolstack(st2);
		}else{
			ERROR("関数でないものを実行しようとしました。");
		}
	}else if(args.type!=TYPE_NULL){
		ERROR("argsにTYPE_CONSまたはTYPE_NULL以外のものが渡されました。");
		fprintf(stderr,"NOTE:渡された引数={type=%d,var=%p}\n",args.type,args.var);
	}
	return v;
}

variable eval(variable c,struct symbolstack* st){
	variable rhead,whead;
	variable r,res;

	if(c.type==TYPE_CONS){
		r=newvariable(TYPE_CONS,0);
		whead=r;
		((cons*)r.var)->car=eval(((cons*)c.var)->car,st);
		if( ((cons*)r.var)->car.type == TYPE_IFUNC || ((cons*)r.var)->car.type == TYPE_LAMBDA){
			for(rhead=((cons*)c.var)->cdr;rhead.type==TYPE_CONS;rhead=((cons*)rhead.var)->cdr){
				((cons*)whead.var)->cdr=newvariable(TYPE_CONS,0);
				whead=((cons*)whead.var)->cdr;
				((cons*)whead.var)->car=eval(((cons*)rhead.var)->car,st);
			}
			((cons*)whead.var)->cdr=(variable){TYPE_NULL,0};
			res=callfunc(((cons*)r.var)->car,((cons*)r.var)->cdr,st);
			delvariable(r);
			return res;
		}else if( ((cons*)r.var)->car.type == TYPE_SPFORM ){
			((cons*)r.var)->cdr=copyvariable(((cons*)c.var)->cdr);
			res = callfunc(((cons*)r.var)->car,((cons*)r.var)->cdr,st);
			return res;
		}else{
			ERROR("関数でないものを実行しようとしました。");
			delvariable(r);
			return (variable){TYPE_NULL,0};
		}
	}else if(c.type==TYPE_SYM){
		return sget((char*)c.var,st);
	}else{
		return copyvariable(c);
	}
}
