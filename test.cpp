/*
    Copyright (C) Daniel Gutson 2013
*/

#include "basicStatements.h"

struct Context
{
    int i;
    int x;
};

int main()
{
    Context ctx { 0, 0 };

    /* The follow program:

    for(i = 1; i < 100; ++i)
        x = x + i;

    */

    ForStatement<
        AssignStatement<int,
            Variable<int, Context, &Context::i>,
            Literal<int, 1>
        >,
        LTComparissonStatement<
            Variable<int, Context, &Context::i>,
            Literal<int, 10>
        >,
        PreIncrStatement<int, Variable<int, Context, &Context::i>>,
        AssignStatement<int,
            Variable<int, Context, &Context::x>,
            AddStatement<int,
                Variable<int, Context, &Context::x>,
                Variable<int, Context, &Context::i>
            >
        >
    > f;

    f(ctx);
    cout << ctx.x << endl;
}

