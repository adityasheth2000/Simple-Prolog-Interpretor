#include<bits/stdc++.h>
using namespace std;

struct Variable
{
    string var_name;
    Variable();
    Variable(string tmp);
    void print();
    bool operator<(const Variable &other) const;
    bool operator==(const Variable &other) const;
};
struct Constant
{
    bool is_int;
    string const_str;
    int const_int;
    Constant(string tmp); 
    Constant(int tmp);
    void print();
};
struct Term;
struct Complex
{
    string functor;
    vector<Term *> subterms;
    Complex(string functor_name, vector<Term *> subterms_list);
    void print();
};
struct Term
{
    Variable *variable;
    Constant *constant;
    Complex *complex;
    Term();
    Term(Variable *tmp);
    Term(Constant *tmp);
    Term(Complex *tmp);
    void print();
};

struct DSU_Variables
{
    map<Variable,Variable> parent_variable;
    map<Variable,int> dsu;
    bool contains_variable(Variable v);
    void insert_variable(Variable v);
    Variable find_parent(Variable a);
    void merge_variables(Variable a, Variable b);
};
/*-----------------------Unification part------------------------------------------*/

bool unification(Term *lhs, Term *rhs, map<Variable, Term*> &variable_assignment);
bool unify(Term *lhs, Term *rhs, map<Variable , Term *> &variable_assignment, DSU_Variables &dsu);
bool check_constant(Constant *lhs, Constant *rhs);
bool unify_complex(Complex *lhs, Complex *rhs, map<Variable , Term *> &variable_assignment, DSU_Variables &dsu);

/* ----------------------Proof search + Knowledge-Base data structure--------------*/

struct Fact
{
    Term* term;
    vector<Fact> dependencies; // term :- dependencies[0], dependencies[1], ... , dependencies.back().
    Fact(Term* t, vector<Fact> dep);
    void print();
};
struct KnowledgeBase
{
    vector<Fact> kb;
    void rec(int index,vector<vector<map<Variable,Term*>>> &dependencies, map<Variable,Term*> variable_assignment,vector<map<Variable,Term*>> &valid_assignments);
    vector<map<Variable,Term*>> Proof_Search(Fact f);
    void insert_Fact(Fact f);
};


/*---------------Parsing and object formations-------------*/

Term* make_Term(string s);
Complex* make_Complex(string s);
Constant* make_Constant(string s);
Variable* make_Variable(string s);