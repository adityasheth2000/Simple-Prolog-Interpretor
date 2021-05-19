#ifndef PROOFSEARCH
#define PROOFSEARCH
#include "ProofSearch.hpp"
#endif

#ifndef UNIFICATION
#define UNIFICATION
#include "Unification.hpp"
#endif

Fact::Fact(Term *t, vector<Fact> dep) : term(t), dependencies(dep) {}

void KnowledgeBase::rec(int index, vector<vector<map<Variable, Term *>>> &dependencies, map<Variable, Term *> variable_assignment, vector<map<Variable, Term *>> &valid_assignments)
{
    if (index == dependencies.size())
    {
        valid_assignments.push_back(variable_assignment);
        return;
    }
    for (int i = 0; i < dependencies[index].size(); i++)
    {
        auto vas = dependencies[index][i];
        bool flag = 1;
        auto cpy_variable_assignment = variable_assignment;
        for (auto x : vas)
        {
            if (cpy_variable_assignment.count(x.first))
            {
                flag = flag & unification(x.second, cpy_variable_assignment[x.first], cpy_variable_assignment);
            }
            if (!flag)
                break;
        }
        if (flag)
        {
            rec(index + 1, dependencies, cpy_variable_assignment, valid_assignments);
        }
    }
}
vector<map<Variable, Term *>> KnowledgeBase::Proof_Search(Fact f)
{
    vector<map<Variable, Term *>> ans;
    for (auto v : kb)
    {
        map<Variable, Term *> variable_assignment;
        if (unification(f.term, v.term, variable_assignment))
        {
            vector<vector<map<Variable, Term *>>> Dependencies_Assignment; //index 0 will hold proof search result of 1st dependency and so on.
            for (int i = 0; i < v.dependencies.size(); i++)
            {
                auto proof_search_result = Proof_Search(v.dependencies[i]);
                Dependencies_Assignment.push_back(proof_search_result);
            }
            rec(0, Dependencies_Assignment, variable_assignment, ans);
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
    cout << " :- ";
    for (auto f : dependencies)
    {
        f.term->print();
        cout << " & ";
    }
}