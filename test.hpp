#include<bits/stdc++.h>
using namespace std;

struct Variable
{
    string var_name;
    Variable();
    Variable(string tmp);
    bool operator<(const Variable &other) const;
    bool operator==(const Variable &other) const;
};