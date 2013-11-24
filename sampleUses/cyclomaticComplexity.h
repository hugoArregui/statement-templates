template <int a, int b>
struct Max
{
    static const int value = a > b ? a : b;
};

template <class T>
struct CyclomaticComplexity
{
    static const int value = 1;
};

template <>
struct CyclomaticComplexity<NOP>
{
    static const int value = 0;
};

template <class Last>
struct CyclomaticComplexity<StatementsList<Last>> 
{
    static const int value = CyclomaticComplexity<Last>::value;
};

template <class Head, class... Tail>
struct CyclomaticComplexity<StatementsList<Head, Tail...>> 
{
    static const int value = Max<CyclomaticComplexity<Head>::value, CyclomaticComplexity<StatementsList<Tail...>>::value>::value;
};

template <class Init, class Condition, class Incr, class Body>
struct CyclomaticComplexity<ForStatement<Init, Condition, Incr, Body>> 
{
    static const int value = 1 + CyclomaticComplexity<Body>::value;
};

template <class Condition, class TruePart, class ElsePart>
struct CyclomaticComplexity<IfStatement<Condition, TruePart, ElsePart>>
{
    static const int value = 1 + Max<CyclomaticComplexity<TruePart>::value, CyclomaticComplexity<ElsePart>::value>::value;
};
