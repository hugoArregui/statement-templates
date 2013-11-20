/*
    Copyright (C) Daniel Gutson 2013
*/

#include "basicStatements.h"

struct Context
{
    int i;
    int x;
    int y;
};

static void testSerialFor()
{
    Context ctx { 0, 0, 0 };

    /* The follow program:

    for(i = 1; i < 100; ++i)
    {
        x = x + i;
        y = x + y;
    }

    */

    ForStatement<
        AssignStatement<int,
            Variable<int, Context, &Context::i>,
            Literal<int, 1>
        >,
        LTComparisonStatement<
            Variable<int, Context, &Context::i>,
            Literal<int, 100>
        >,
        PreIncrStatement<int, Variable<int, Context, &Context::i>>,
        StatementsList<
            AssignStatement<int,
                Variable<int, Context, &Context::x>,
                AddStatement<int,
                    Variable<int, Context, &Context::x>,
                    Variable<int, Context, &Context::i>
                >
            >,
            AssignStatement<int,
                Variable<int, Context, &Context::y>,
                AddStatement<int,
                    Variable<int, Context, &Context::x>,
                    Variable<int, Context, &Context::y>
                >
            >
        >
    > f;

    f(ctx);
    cout << "x: " << ctx.x << endl << "y: " << ctx.y << endl;
}

int main()
{
    testSerialFor();
}

