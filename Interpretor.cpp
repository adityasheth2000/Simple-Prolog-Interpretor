#include "Interpretor.hpp"
#include "debug_template.cpp"

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

/* ----------------------Proof search + Knowledge-Base data structure--------------*/

Fact::Fact(Term* t,vector<Fact> dep):term(t),dependencies(dep){}

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
void KnowledgeBase::insert_Fact(Fact f)
{
    kb.push_back(f);
}
void Fact::print()
{
    term->print();
    cout<<" :- ";
    for(auto f: dependencies)
    {
        f.term->print();
        cout<<" & ";
    }
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


/*-------Parsing and object formations----------*/

Term* make_Term(string s)
{
    // tr(s);
    assert(s.length()>0);//No case should happen when s becomes empty;
    Term *ans;
    if(s.back()==')')//Complex
    {       
        Complex *cplx=make_Complex(s);
        ans=new Term(cplx);   
    }
    else if(s[0]>='A' && s[0]<='Z')//Variable
    {
        Variable *v=make_Variable(s);
        ans=new Term(v);
    }
    else//Constant
    {
        Constant *c=make_Constant(s);
        ans=new Term(c);
    }
    return ans;
}
Complex* make_Complex(string s)
{
    assert(s[0]!='(');//functor name is required
    assert(s.back()==')');
    int n=s.length();
    bool check=0;
    string functor="";
    int index_open=-1;
    for(int i=0;i<n;i++)
    {
        if(s[i]=='('){
            index_open=i;
            break;
        }
        else{
            functor+=s[i];
        }
    }
    assert(index_open>=0);
    vector<int> index_comma;
    int depth=0;
    for(int i=0;i<n;i++)
    {
        if(s[i]=='(')depth++;
        else if(s[i]==')')depth--;
        if(s[i]==',' && depth==1)
        {
            index_comma.push_back(i);
        }
    }
    int cur=index_open+1;
    vector<Term*> v;
    for(int ind: index_comma)
    {
        int l=cur;
        int r=ind-1;
        assert(l<=r);
        v.push_back(make_Term(s.substr(l,r-l+1)));
        cur=ind+1;
    }
    v.push_back(make_Term(s.substr(cur,(int)s.size()-1-cur)));
    Complex *ans=new Complex(functor,v);
    return ans;
}
Constant* make_Constant(string s)
{
    bool var=1;
    for(int i=0;i<s.length() && var;i++)
    {
        if(s[i]<'0' || s[i]>'1')
            var=0;
    }
    Constant *c;
    if(var)
    {
        c=new Constant(stoi(s));
    }
    else
    {
        c=new Constant(s);
    }
    return c;
}
Variable* make_Variable(string s)
{
    Variable *v=new Variable(s);
    return v;
}

int main()
{ 
    
    map<Variable, Term *> variable_assignment;
    Term* lhs=make_Term("h(X,Y,g(X,Y))");
    Term* rhs=make_Term("h(a,a,g(Y,X))");
    bool f = unification(lhs,rhs, variable_assignment);
    tr(f);
    if (f)
        print_variable_assignment(variable_assignment);
}
