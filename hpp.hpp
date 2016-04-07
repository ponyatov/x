#ifndef _H_X
#define _H_X

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string tag,val;
	Sym(string,string); Sym(string);
	vector<Sym*> nest; void push(Sym*o);
	map<string,Sym*> pars;
	virtual string tagval(); string tagstr();
	virtual string dump(int=0); string pad(int);
	virtual Sym* eval();
	virtual Sym* eq(Sym*);
	virtual Sym* at(Sym*);
//	virtual Sym* add(Sym*);
	static Sym* fn_h(Sym*); virtual Sym* h();
	static Sym* fn_c(Sym*); virtual Sym* c();
	virtual Sym* colon(Sym*);
	virtual Sym* perc(Sym*);
};

extern map<string,Sym*> env;
extern void env_init();

struct Str: Sym { Str(string); string tagval(); };

struct Op: Sym { Op(string); Sym*eval(); Sym*c(); };

typedef Sym*(*FN)(Sym*);
struct Fn: Sym { Fn(string,FN); FN fn; Sym*at(Sym*); };

struct Class: Sym { Class(string,string); Sym*colon(Sym*); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_X
