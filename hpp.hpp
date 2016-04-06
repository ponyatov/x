#ifndef _H_X
#define _H_X

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string tag,val;
	Sym(string,string); Sym(string);
	vector<Sym*> nest; void push(Sym*o);
	virtual string dump(int=0); virtual string tagval(); string pad(int);
};

struct Op: Sym { Op(string); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_X
