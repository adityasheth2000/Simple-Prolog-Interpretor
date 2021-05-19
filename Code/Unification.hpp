#ifndef STRUCTURES
#define STRUCTURES
#include "Structures.hpp"
#endif

bool unification(Term *lhs, Term *rhs, map<Variable, Term *> &variable_assignment);
bool unify(Term *lhs, Term *rhs, map<Variable, Term *> &variable_assignment, DSU_Variables &dsu);
bool check_constant(Constant *lhs, Constant *rhs);
bool unify_complex(Complex *lhs, Complex *rhs, map<Variable, Term *> &variable_assignment, DSU_Variables &dsu);
