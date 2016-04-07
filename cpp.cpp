#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

Sym::Sym(string T, string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }
void Sym::par(Sym*o) { pars[o->str()]=o; }

string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::tagval() { return "<"+tag+":"+val+">"; }
string Sym::str() { return val; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+tagval();
	for (auto pr=pars.begin(),e=pars.end();pr!=e;pr++)
		S += "," + pr->first + "=" + pr->second->tagval();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym* E = env[val]; if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it)=(*it)->eval();
	return this;
}

Sym* Sym::eq(Sym*o) { env[val]=o; return o; }
Sym* Sym::at(Sym*o) { push(o); return this; }

Sym* Sym::colon(Sym*o) { push(o); return this; }
Sym* Sym::perc(Sym*o) { push(o); return this; }

Sym* Sym::add(Sym*o) { return new Sym(val+"+"+o->str()); }

Sym* Sym::fn_h(Sym*o) { return o->h(); }
Sym* Sym::fn_c(Sym*o) { return o->c(); }

Sym* Sym::h() { return new Str("extern Sym "+val+";"); }
Sym* Sym::c() { return new Str(val); }
string Str::tagval() { return tagstr(); }
string Sym::tagstr() { string S = "'";
	for (int i=0,e=val.length();i!=e;i++)
		switch (val[i]) {
//		case '\t': S+="\\t"; break;
//		case '\n': S+="\\n"; break;
		default: S+=val[i];
		}
	return S+"'"; }

Str::Str(string V):Sym("str",V) {}
Sym* Str::add(Sym*o) { return new Str(val+o->str()); }

List::List():Sym("[]") {}
Sym* List::c() { string S;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->c()->str()+"\n";
	return new Str(S); }

Op::Op(string V):Sym("op",V) {}
Sym* Op::eval() {
	if (val=="~") return nest[0]; else Sym::eval();
	if (val=="=") return nest[0]->eq(nest[1]);
	if (val=="@") return nest[0]->at(nest[1]);
	if (val==":") return nest[0]->colon(nest[1]);
	if (val=="%") return nest[0]->perc(nest[1]);
	if (val=="+") return nest[0]->add(nest[1]);
	return this;
}
Sym* Op::c() { ostringstream os;
	if (val=="@") {
		os << nest[0]->c()->str();
		os << "( " << nest[1]->c()->str() << " )";
		return new Str(os.str());
	}
	os << "(" << nest[0]->c()->str() << ")";
	os << val;
	os << "(" << nest[1]->c()->str() << ")";
	return new Str(os.str()); }

Fn::Fn(string V, FN F):Sym("fn",V) { fn=F; }
Sym* Fn::at(Sym*o) { return fn(o); }

Lambda::Lambda():Sym("^","^") {}
Sym* Lambda::eval() { return this; }

Class::Class(string T, string V):Sym(T,V) {}
Sym* Class::colon(Sym*o) {
	Class*C = new Class(val,o->str()); C->pars["super"]=this;
	return C; }

Class base("class","class");

map<string,Sym*> env;
void env_init() {
	env["h"] = new Fn("h",Sym::fn_h);
	env["c"] = new Fn("c",Sym::fn_c);
	env["class"] = &base; base.pars["super"]=&base;
}
