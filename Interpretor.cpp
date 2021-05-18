#include "Interpretor.hpp"
// #include<bits/stdc++.h>
// using namespace std;
#define t1(x) cerr << #x << "=" << x << endl
#define t2(x, y) cerr << #x << "=" << x << " " << #y << "=" << y << endl
#define t3(x, y, z) cerr << #x << "=" << x << " " << #y << "=" << y << " " << #z << "=" << z << endl
#define t4(a, b, c, d) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << endl
#define t5(a, b, c, d, e) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << " " << #e << "=" << e << endl
#define t6(a, b, c, d, e, f) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << " " << #e << "=" << e << " " << #f << "=" << f << endl
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define tr(...)                                    \
    GET_MACRO(__VA_ARGS__, t6, t5, t4, t3, t2, t1) \
    (__VA_ARGS__)

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

/*-----------------------Unification part------------------------------------------*/
bool check_constant(Constant *lhs, Constant *rhs)
{
    return (lhs->const_int == rhs->const_int) && (lhs->const_str == rhs->const_str);
}
bool unification(Term *lhs, Term *rhs, map<Variable, Term*> &variable_assignment)
{    
    DSU_Variables dsu;
    if(!unify(lhs,rhs,variable_assignment,dsu))
        return false;   
    bool flag=1;
    for(auto x: variable_assignment){
        auto v=x.first;
        if(!dsu.contains_variable(v))
            continue;
        map<Variable,Term*> tmp_va;
        flag &= unification(x.second,variable_assignment[dsu.find_parent(v)], tmp_va);
    }
    return flag;
}
bool unify(Term *lhs, Term *rhs, map<Variable , Term *> &variable_assignment, DSU_Variables &dsu)
{
    if (lhs->variable && rhs->variable)
    {
        // tr("here1");
        if(!dsu.contains_variable(*(lhs->variable)))
            dsu.insert_variable(*(lhs->variable));   
        if(!dsu.contains_variable(*(rhs->variable)))
            dsu.insert_variable(*(rhs->variable));
        dsu.merge_variables(*(lhs->variable),*(rhs->variable));
        if(variable_assignment.count(*(lhs->variable)) && variable_assignment.count(*(rhs->variable)))
        {
            return unification(variable_assignment[*(lhs->variable)],variable_assignment[*(rhs->variable)],variable_assignment);
        }
        return true;
    }
    else if (lhs->variable && !rhs->variable)
    {       
        // tr("here2");
        if(variable_assignment.count(*(lhs->variable))){
            map<Variable,Term*> tmp_va;
            return unification(variable_assignment[*(lhs->variable)],rhs, tmp_va);
        }
        variable_assignment[*(lhs->variable)] = rhs;
    }
    else if (rhs->variable && !lhs->variable)
    {
        // tr("here3");
        if(variable_assignment.count(*(rhs->variable))){
            map<Variable,Term*> tmp_va;
            return unification(variable_assignment[*(rhs->variable)],lhs,tmp_va);
        }
        variable_assignment[*(rhs->variable)] = lhs;
    }
    else if (lhs->complex && rhs->complex)
    {
        // tr("here4");
        bool f1=unify_complex(lhs->complex, rhs->complex, variable_assignment, dsu);
        // tr(f1);
        return f1;
    }
    else if (lhs->constant && rhs->constant)
    {
        return check_constant(lhs->constant, rhs->constant);
    }
    else
    {
        return false;
    }
    return true;
}

bool unify_complex(Complex *lhs, Complex *rhs, map<Variable , Term *> &variable_assignment, DSU_Variables &dsu)
{
    if (lhs->functor != rhs->functor)
        return false;
    if (lhs->subterms.size() != rhs->subterms.size())
        return false;
    bool flag = 1;
    for (int i = 0; i < lhs->subterms.size() && flag; i++){
        flag &= unify(lhs->subterms[i], rhs->subterms[i], variable_assignment, dsu);
    }
    return flag;
}
    
void KnowledgeBase::rec(int index,vector<vector<map<Variable,Term*>>> &dependencies, map<Variable,Term*> variable_assignment,vector<map<Variable,Term*>> &valid_assignments)
{
    if(index==dependencies.size()){
        valid_assignments.push_back(variable_assignment);
        return;
    }
    for(int i=0;i<dependencies[index].size();i++)
    {
        auto vas=dependencies[index][i];
        bool flag=1;
        auto cpy_variable_assignment=variable_assignment;
        for(auto x: vas)
        {
            if(cpy_variable_assignment.count(x.first)){
                flag=flag&unification(x.second,cpy_variable_assignment[x.first],cpy_variable_assignment);
            }
            if(!flag)
                break;
        }
        if(flag)
        {
            rec(index+1,dependencies,cpy_variable_assignment,valid_assignments);
        }
    }
}
vector<map<Variable,Term*>> KnowledgeBase::Proof_Search(Fact f)
{
    vector<map<Variable,Term*>> ans;
    for(auto v: kb)
    {
        map<Variable,Term*> variable_assignment;
        if(unification(f.term, v.term, variable_assignment))
        {
            vector<vector<map<Variable,Term*>>> Dependencies_Assignment;//index 0 will hold proof search result of 1st dependency and so on.
            for(int i=0;i<v.dependencies.size();i++){
                auto proof_search_result=Proof_Search(v.dependencies[i]);
                Dependencies_Assignment.push_back(proof_search_result);
            }
            rec(0,Dependencies_Assignment,variable_assignment,ans);
        }
    }    
    return ans;
}

void print_variable_assignment(map<Variable,Term*> &variable_assignment)
{
    for(auto x: variable_assignment)
    {
        Variable v=x.first;
        Term* t=x.second;
        v.print();
        cout<<" = ";
        t->print();
        cout<<"; ";
    }
    cout<<endl;
}

int main()
{ 
    
    
}

/*
Todo : 
Knowledge base construction
Unification,
Query 
*/