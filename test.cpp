#include "test.hpp"

Variable::Variable()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int r = rng();
    var_name = "Unknown_" + to_string(r);
}
Variable::Variable(string tmp) : var_name(tmp) {}

bool Variable::operator<(const Variable &other) const
{
    return var_name < other.var_name;
}
bool Variable::operator==(const Variable &other) const
{
    return var_name == other.var_name;
}

int main()
{
}