#ifndef STRUCTURES
#define STRUCTURES
#include "Structures.hpp"
#endif


Variable::Variable()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int r=rng();
    var_name="Unknown_"+to_string(r);
}
Variable::Variable(string tmp) : var_name(tmp){}
void Variable::print()
{
    cout<<var_name;
}
bool Variable::operator<(const Variable &other) const
{
    return var_name<other.var_name;
}
bool Variable::operator==(const Variable &other) const
{
    return var_name==other.var_name;
}


Constant::Constant(string tmp) : const_str(tmp), const_int(0),is_int(false) {}
Constant::Constant(int tmp) : const_int(tmp), const_str(""),is_int(true) {}
void Constant::print()
{
    if(is_int)
        cout<<const_int;
    else
        cout<<const_str;
}


Complex::Complex(string functor_name, vector<Term *> subterms_list) : functor(functor_name), subterms(subterms_list) {}
void Complex::print()
{
    cout<<functor<<"(";
    for(int i=0;i<subterms.size();i++)
    {
        subterms[i]->print();
        if(i+1<subterms.size())
            cout<<",";
    }
    cout<<")";
}


Term::Term() : variable(NULL), constant(NULL), complex(NULL) {}
Term::Term(Variable *tmp) : variable(tmp), constant(NULL), complex(NULL) {}
Term::Term(Constant *tmp) : variable(NULL), constant(tmp), complex(NULL) {}
Term::Term(Complex *tmp) : variable(NULL), constant(NULL), complex(tmp) {}
void Term::print()
{
    if(variable)
    {
        variable->print();
    }
    else if(constant)
    {
        constant->print();
    }
    else if(complex)
    {
        complex->print();
    }
}



bool DSU_Variables::contains_variable(Variable v){
    return (dsu.count(v)>0);
}
void DSU_Variables::insert_variable(Variable v){
    parent_variable[v]=Variable("Temporary_variable");
    dsu[v]=1;
}
Variable DSU_Variables::find_parent(Variable a){
    Variable v=a;
    
    while(dsu[v]==0){
        v=parent_variable[v];
    }
    return v;
}
void DSU_Variables::merge_variables(Variable a, Variable b)
{
    a=find_parent(a);
    b=find_parent(b);
    int sz_a=dsu[a];
    int sz_b=dsu[b];
    if(a==b)
        return;
    if(sz_a<=sz_b){
        parent_variable[a]=b;
        dsu[b]+=sz_a;
        dsu[a]=0;
    }
    else{
        parent_variable[a]=a;
        dsu[a]+=sz_b;
        dsu[b]=0;
    }
}    
