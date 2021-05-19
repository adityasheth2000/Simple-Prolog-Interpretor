#include "debug_template.cpp"

#include "parser.cpp"
#include "ProofSearch.cpp"
#include "Structures.cpp"
#include "Unification.cpp"

void print_variable_assignment(map<Variable, Term *> &variable_assignment)
{
    for (auto x : variable_assignment)
    {
        Variable v = x.first;
        Term *t = x.second;
        v.print();
        cout << " = ";
        t->print();
        cout << "; ";
    }
    cout << endl;
}

/*-------Parsing and object formations----------*/

void proof_search_result_output(vector<map<Variable, Term *>> ans)
{
    if (ans.size() == 0)
    {
        cout << "no results!";
        return;
    }
    for (auto vas : ans)
    {
        cout << "{\n";
        for (auto x : vas)
        {
            cout << "\t";
            Variable v = x.first;
            v.print();
            cout << " : ";
            x.second->print();
            cout << endl;
        }
        cout << "}\n";
    }
}

int main()
{

    // {
    //     map<Variable, Term *> variable_assignment;
    //     Term* lhs=make_Term("");
    //     Term* rhs=make_Term("");
    //     lhs->print();
    //     cout<<endl;
    //     rhs->print();
    //     cout<<endl;
    //     bool f = unification(lhs,rhs, variable_assignment);
    //     tr(f);
    //     if (f)
    //         print_variable_assignment(variable_assignment);
    // }

    {
        // happy(yolanda).
        // listens2Music(mia).
        // listens2Music(yolanda):-  happy(yolanda).
        // playsAirGuitar(mia):-  listens2Music(mia).
        // playsAirGuitar(yolanda):-  listens2Music(yolanda).

        Fact f1 = Fact(make_Term("happy(yolanda)"), {});
        Fact f2 = Fact(make_Term("listens2Music(mia)"), {});
        Fact f3 = Fact(make_Term("listens2Music(yolanda)"), {Fact(make_Term("happy(yolanda)"), {})});
        Fact f4 = Fact(make_Term("playsAirGuitar(mia)"), {Fact(make_Term("listens2Music(mia)"), {})});
        Fact f5 = Fact(make_Term("playsAirGuitar(yolanda)"), {Fact(make_Term("listens2Music(yolanda)"), {})});

        KnowledgeBase kb;
        kb.insert_Fact(f1);
        kb.insert_Fact(f2);
        kb.insert_Fact(f3);
        kb.insert_Fact(f4);
        kb.insert_Fact(f5);

        Fact qry = Fact(make_Term("listens2Music(X)"), {});
        auto ans = kb.Proof_Search(qry);
        proof_search_result_output(ans);
    }
}
