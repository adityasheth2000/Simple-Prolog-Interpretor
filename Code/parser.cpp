#ifndef PARSER
#define PARSER
#include "parser.hpp"
#endif

Term *make_Term(string s)
{
    // tr(s);
    assert(s.length() > 0); //No case should happen when s becomes empty;
    Term *ans;
    if (s.back() == ')') //Complex
    {
        Complex *cplx = make_Complex(s);
        ans = new Term(cplx);
    }
    else if (s[0] >= 'A' && s[0] <= 'Z') //Variable
    {
        Variable *v = make_Variable(s);
        ans = new Term(v);
    }
    else //Constant
    {
        Constant *c = make_Constant(s);
        ans = new Term(c);
    }
    return ans;
}
Complex *make_Complex(string s)
{
    assert(s[0] != '('); //functor name is required
    assert(s.back() == ')');
    int n = s.length();
    bool check = 0;
    string functor = "";
    int index_open = -1;
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '(')
        {
            index_open = i;
            break;
        }
        else
        {
            functor += s[i];
        }
    }
    assert(index_open >= 0);
    vector<int> index_comma;
    int depth = 0;
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '(')
            depth++;
        else if (s[i] == ')')
            depth--;
        if (s[i] == ',' && depth == 1)
        {
            index_comma.push_back(i);
        }
    }
    int cur = index_open + 1;
    vector<Term *> v;
    for (int ind : index_comma)
    {
        int l = cur;
        int r = ind - 1;
        assert(l <= r);
        v.push_back(make_Term(s.substr(l, r - l + 1)));
        cur = ind + 1;
    }
    v.push_back(make_Term(s.substr(cur, (int)s.size() - 1 - cur)));
    Complex *ans = new Complex(functor, v);
    return ans;
}
Constant *make_Constant(string s)
{
    bool var = 1;
    for (int i = 0; i < s.length() && var; i++)
    {
        if (s[i] < '0' || s[i] > '1')
            var = 0;
    }
    Constant *c;
    if (var)
    {
        c = new Constant(stoi(s));
    }
    else
    {
        c = new Constant(s);
    }
    return c;
}
Variable *make_Variable(string s)
{
    Variable *v = new Variable(s);
    return v;
}
