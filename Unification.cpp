#ifndef UNIFICATION
#define UNIFICATION
#include "Unification.hpp"
#endif

bool check_constant(Constant *lhs, Constant *rhs)
{
    return (lhs->const_int == rhs->const_int) && (lhs->const_str == rhs->const_str);
}
bool unification(Term *lhs, Term *rhs, map<Variable, Term *> &variable_assignment)
{
    DSU_Variables dsu;
    if (!unify(lhs, rhs, variable_assignment, dsu))
        return false;
    bool flag = 1;
    for (auto x : variable_assignment)
    {
        auto v = x.first;
        if (!dsu.contains_variable(v))
            continue;
        map<Variable, Term *> tmp_va;
        flag &= unification(x.second, variable_assignment[dsu.find_parent(v)], tmp_va);
    }
    return flag;
}
bool unify(Term *lhs, Term *rhs, map<Variable, Term *> &variable_assignment, DSU_Variables &dsu)
{
    if (lhs->variable && rhs->variable)
    {
        // tr("here1");
        if (!dsu.contains_variable(*(lhs->variable)))
            dsu.insert_variable(*(lhs->variable));
        if (!dsu.contains_variable(*(rhs->variable)))
            dsu.insert_variable(*(rhs->variable));
        dsu.merge_variables(*(lhs->variable), *(rhs->variable));
        if (variable_assignment.count(*(lhs->variable)) && variable_assignment.count(*(rhs->variable)))
        {
            return unification(variable_assignment[*(lhs->variable)], variable_assignment[*(rhs->variable)], variable_assignment);
        }
        return true;
    }
    else if (lhs->variable && !rhs->variable)
    {
        // tr("here2");
        if (variable_assignment.count(*(lhs->variable)))
        {
            map<Variable, Term *> tmp_va;
            return unification(variable_assignment[*(lhs->variable)], rhs, tmp_va);
        }
        variable_assignment[*(lhs->variable)] = rhs;
    }
    else if (rhs->variable && !lhs->variable)
    {
        // tr("here3");
        if (variable_assignment.count(*(rhs->variable)))
        {
            map<Variable, Term *> tmp_va;
            return unification(variable_assignment[*(rhs->variable)], lhs, tmp_va);
        }
        variable_assignment[*(rhs->variable)] = lhs;
    }
    else if (lhs->complex && rhs->complex)
    {
        // tr("here4");
        bool f1 = unify_complex(lhs->complex, rhs->complex, variable_assignment, dsu);
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

bool unify_complex(Complex *lhs, Complex *rhs, map<Variable, Term *> &variable_assignment, DSU_Variables &dsu)
{
    if (lhs->functor != rhs->functor)
        return false;
    if (lhs->subterms.size() != rhs->subterms.size())
        return false;
    bool flag = 1;
    for (int i = 0; i < lhs->subterms.size() && flag; i++)
    {
        flag &= unify(lhs->subterms[i], rhs->subterms[i], variable_assignment, dsu);
    }
    return flag;
}
