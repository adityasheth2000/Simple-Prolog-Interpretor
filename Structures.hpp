#include <bits/stdc++.h>
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
    map<Variable, Variable> parent_variable;
    map<Variable, int> dsu;
    bool contains_variable(Variable v);
    void insert_variable(Variable v);
    Variable find_parent(Variable a);
    void merge_variables(Variable a, Variable b);
};
