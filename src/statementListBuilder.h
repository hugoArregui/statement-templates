
template <class ... Statements>
struct StatementsListBuilder;

template <class Last>
struct StatementsListBuilder<Last> : Last
{
    typedef StatementsList<Last, NOP> Type;
};


template <class Head, class... Tail>
struct StatementsListBuilder<Head, Tail...> : StatementsListBuilder<Tail...>
{
    typedef StatementsList<Head, typename StatementsListBuilder<Tail...>::Type> Type;
};
