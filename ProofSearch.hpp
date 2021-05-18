#ifndef STRUCTURES
#define STRUCTURES
#include "Structures.hpp"
#endif

struct Fact
{
    Term *term;
    vector<Fact> dependencies; // term :- dependencies[0], dependencies[1], ... , dependencies.back().
    Fact(Term *t, vector<Fact> dep);
    void print();
};
struct KnowledgeBase
{
    vector<Fact> kb;
    void rec(int index, vector<vector<map<Variable, Term *>>> &dependencies, map<Variable, Term *> variable_assignment, vector<map<Variable, Term *>> &valid_assignments);
    vector<map<Variable, Term *>> Proof_Search(Fact f);
    void insert_Fact(Fact f);
};