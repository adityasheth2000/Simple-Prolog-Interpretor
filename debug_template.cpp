#define t1(x) cerr << #x << "=" << x << endl
#define t2(x, y) cerr << #x << "=" << x << " " << #y << "=" << y << endl
#define t3(x, y, z) cerr << #x << "=" << x << " " << #y << "=" << y << " " << #z << "=" << z << endl
#define t4(a, b, c, d) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << endl
#define t5(a, b, c, d, e) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << " " << #e << "=" << e << endl
#define t6(a, b, c, d, e, f) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << " " << #e << "=" << e << " " << #f << "=" << f << endl
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define tr(...)                                    \
    GET_MACRO(__VA_ARGS__, t6, t5, t4, t3, t2, t1) \
    (__VA_ARGS__)
    