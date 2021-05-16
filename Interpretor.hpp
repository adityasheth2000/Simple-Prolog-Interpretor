#include <bits/stdc++.h>
using namespace std;

struct Term;
struct Variable;
struct Constant;
struct Complex;

struct KnowledgeBase;
struct DSU_Variables;

bool unification(Term *lhs, Term *rhs, map<Variable, Term *> &variable_assignment);
bool unify(Term *lhs, Term *rhs, map<Variable, Term *> &variable_assignment, DSU_Variables &dsu);
bool unify_complex(Complex *lhs, Complex *rhs, map<Variable, Term *> &variable_assignment, DSU_Variables &dsu);
bool check_constant(Constant *lhs, Constant *rhs);
