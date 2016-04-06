#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { return yyparse(); }

Sym::Sym(string T, string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::tagval() { return "<"+tag+":"+val+">"; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+tagval();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }


Op::Op(string V):Sym("op",V) {}
