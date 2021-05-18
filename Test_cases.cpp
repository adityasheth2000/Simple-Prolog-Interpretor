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

    {
        //h(X,Y,g(X,Y)) = h(a,a,g(Y,X))
        Variable x("X"),y("Y");
        Term tx=Term(&x);
        Term ty=Term(&y);
        Complex cl=Complex("g",{&tx,&ty});
        Complex cr=Complex("g",{&ty,&tx});
        Constant cnst=Constant("a");

        Term tcl=Term(&cl);
        Term tcr=Term(&cr);
        Term tcnst=Term(&cnst);

        Complex lft=Complex("h",{&tx,&ty,&tcl});
        Complex rght=Complex("h",{&tcnst,&tcnst,&tcr});

        Term lhs=Term(&lft);
        Term rhs=Term(&rght);
        
        // lhs.print();
        // rhs.print();
        
        map<Variable, Term *> variable_assignment;
        bool f = unification(&lhs, &rhs, variable_assignment);
        tr(f);
        print_variable_assignment(variable_assignment);
    }

    {
        /*
            h(vincent).
            l2m(butch).
            pag(vincent):- l2m(vincent),h(vincent).
            pag(butch):- h(butch).
            pag(butch):- l2m(butch).

            ProofSearch(h(X)); 
        */
        
        Constant c1=Constant("vincent");
        Term tc1=Term(&c1);
        Complex cp1=Complex("h",{&tc1});
        Term t1=Term(&cp1);
        Fact f1=Fact(&t1,{});

        Constant c2=Constant("butch");
        Term tc2=Term(&c2);
        Complex cp2=Complex("l2m",{&tc2});
        Term t2=Term(&cp2);
        Fact f2=Fact(&t2,{});

        Constant c3=Constant("vincent");
        Term tc3=Term(&c3);
        Complex cp3=Complex("pag",{&tc3});
        Term t3=Term(&cp3);        
        Constant c31=Constant("vincent");
        Term tc31=Term(&c31);
        Complex cp31=Complex("l2m",{&tc31});
        Term t31=Term(&cp31);
        Fact f31=Fact(&t31,{});        
        Fact f3=Fact(&t3,{f1,f31});

        Constant c4=Constant("butch");
        Term tc4=Term(&c4);
        Complex cp4=Complex("pag",{&tc4});
        Term t4=Term(&cp4);        
        Constant c41=Constant("butch");
        Term tc41=Term(&c41);
        Complex cp41=Complex("h",{&tc41});
        Term t41=Term(&cp41);
        Fact f41=Fact(&t41,{});        
        Fact f4=Fact(&t4,{f41});


        Constant c5=Constant("butch");
        Term tc5=Term(&c4);
        Complex cp5=Complex("pag",{&tc5});
        Term t5=Term(&cp4);        
        Constant c51=Constant("butch");
        Term tc51=Term(&c51);
        Complex cp51=Complex("l2m",{&tc51});
        Term t51=Term(&cp51);
        Fact f51=Fact(&t51,{});        
        Fact f5=Fact(&t5,{f51});
        
        KnowledgeBase kb;
        kb.insert_Fact(f1);
        kb.insert_Fact(f2);
        kb.insert_Fact(f3);
        kb.insert_Fact(f4);
        kb.insert_Fact(f5);
        
        Variable x=Variable("X");
        Term tx=Term(&x);
        Complex cp=Complex("h",{&tx});        
        Term tcp=Term(&cp);
        Fact qry=Fact(&tcp,{});
        
        auto ans=kb.Proof_Search(qry);
        for(auto vas: ans)
        {
            cout<<"{\n";
            for(auto x: vas)
            {
                cout<<"\t";
                Variable v=x.first;
                v.print();
                cout<<" : ";
                x.second->print();
                cout<<endl;
            }
            cout<<"}\n";
        }              
    }

    {
        /*
            h(vincent).
            l2m(butch).
            pag(vincent):- l2m(vincent),h(vincent).
            pag(butch):- h(butch).
            pag(butch):- l2m(butch). 

            ProofSearch(h(vincent))
            comment: returns non-empty assignment to vector denoting that h(vincent) exist in knowledgebase.
        */
        
        Constant c1=Constant("vincent");
        Term tc1=Term(&c1);
        Complex cp1=Complex("h",{&tc1});
        Term t1=Term(&cp1);
        Fact f1=Fact(&t1,{});

        Constant c2=Constant("butch");
        Term tc2=Term(&c2);
        Complex cp2=Complex("l2m",{&tc2});
        Term t2=Term(&cp2);
        Fact f2=Fact(&t2,{});

        Constant c3=Constant("vincent");
        Term tc3=Term(&c3);
        Complex cp3=Complex("pag",{&tc3});
        Term t3=Term(&cp3);        
        Constant c31=Constant("vincent");
        Term tc31=Term(&c31);
        Complex cp31=Complex("l2m",{&tc31});
        Term t31=Term(&cp31);
        Fact f31=Fact(&t31,{});        
        Fact f3=Fact(&t3,{f1,f31});

        Constant c4=Constant("butch");
        Term tc4=Term(&c4);
        Complex cp4=Complex("pag",{&tc4});
        Term t4=Term(&cp4);        
        Constant c41=Constant("butch");
        Term tc41=Term(&c41);
        Complex cp41=Complex("h",{&tc41});
        Term t41=Term(&cp41);
        Fact f41=Fact(&t41,{});        
        Fact f4=Fact(&t4,{f41});


        Constant c5=Constant("butch");
        Term tc5=Term(&c4);
        Complex cp5=Complex("pag",{&tc5});
        Term t5=Term(&cp4);        
        Constant c51=Constant("butch");
        Term tc51=Term(&c51);
        Complex cp51=Complex("l2m",{&tc51});
        Term t51=Term(&cp51);
        Fact f51=Fact(&t51,{});        
        Fact f5=Fact(&t5,{f51});
        
        KnowledgeBase kb;
        kb.insert_Fact(f1);
        kb.insert_Fact(f2);
        kb.insert_Fact(f3);
        kb.insert_Fact(f4);
        kb.insert_Fact(f5);
        
        Constant x=Constant("vincent");
        Term tx=Term(&x);
        Complex cp=Complex("h",{&tx});        
        Term tcp=Term(&cp);
        Fact qry=Fact(&tcp,{});
        
        auto ans=kb.Proof_Search(qry);
        for(auto vas: ans)
        {
            cout<<"{\n";
            for(auto x: vas)
            {
                cout<<"\t";
                Variable v=x.first;
                v.print();
                cout<<" : ";
                x.second->print();
                cout<<endl;
            }
            cout<<"}\n";
        }      
        
    }
}