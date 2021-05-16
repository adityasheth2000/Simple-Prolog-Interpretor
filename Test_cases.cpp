#include"Interpretor.cpp"
#include<bits/stdc++.h>
using namespace std;
int main()
{

    {
        // X = 1, unification
        Variable X = Variable("X");
        Term lhs = Term(&X);
        Constant One = Constant(1);
        Term rhs = Term(&One);
        map<Variable, Term *> variable_assignment;
        bool f = unification(&lhs, &rhs, variable_assignment);
        cout << "f=" << f << endl;
        for (auto variable : variable_assignment)
        {
            Variable v = variable.first;
            v.print();
            cout << ": ";
            variable.second->print();
            cout << ";";
        }
        cout << endl;
    }
    {
        // f(X,5)=f(4,5), unification

        Constant c5(5);
        Term ct5(&c5);

        Constant c4(4);
        Term ct4(&c4);

        Variable x("X");
        Term vx(&x);

        Complex cplx_left("f", {&vx, &ct5});
        Complex cplx_right("f", {&ct4, &ct5});

        Term lhs(&cplx_left), rhs(&cplx_right);
        map<Variable, Term *> variable_assignment;
        bool f = unification(&lhs, &rhs, variable_assignment);
        tr(f);
        print_variable_assignment(variable_assignment);
    }
    {
        // k(s("g"),Y)  =  k(X,t("k"))
        Variable vx("X"), vy("Y");
        Term tvx(&vx), tvy(&vy);
        Constant cg("g"), ck("k");
        Term tg(&cg), tk(&ck);
        Complex cpg("s", {&tg}), cpk("t", {&tk});
        Term tcpg(&cpg), tcpk(&cpk); //tcpg=term(s("g"))
        Complex lc("k", {&tcpg, &tvy});
        Complex rc("k", {&tvx, &tcpk});

        Term lhs(&lc), rhs(&rc);

        // lhs.print();
        // rhs.print();

        map<Variable, Term *> variable_assignment;
        bool f = unification(&lhs, &rhs, variable_assignment);
        tr(f);
        print_variable_assignment(variable_assignment);
    }
    {
        //loves(X,X)=loves("dog","cat")
        Variable x("X");
        Term tx(&x);

        Constant d("dog");
        Constant c("cat");

        Term tc(&c), td(&d);
        Complex l("loves", {&tx, &tx});
        Complex r("loves", {&tc, &td});
        Term lhs(&l), rhs(&r);

        // lhs.print();
        // rhs.print();

        map<Variable, Term *> variable_assignment;
        bool f = unification(&lhs, &rhs, variable_assignment);
        tr(f);
        if (f)
            print_variable_assignment(variable_assignment);
    }
}