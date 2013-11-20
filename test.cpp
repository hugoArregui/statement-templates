/*
    Copyright (C) Daniel Gutson 2013
*/

#include "basicStatements.h"
#include "parallelFor.h"

struct Context
{
    int i;
    int x;
    int y;
};

struct MyStatement : StatementBase<void>
{
    template <class T>
    void operator()(T& context)
    {
        cout << "i = " << context.i << endl;
    }
};


static void testSerialFor()
{
    Context ctx { 0, 0, 0 };

    /* The follow program:

    for(i = 1; i < 100; ++i)
    {
        x = x + i;
        y = x + y;
        myStatement;
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
            >,
            MyStatement
        >
    > f;

    f(ctx);
    cout << "x: " << ctx.x << endl << "y: " << ctx.y << endl;
}

static void testParallelFor()
{
    Context ctx { 0, 0, 0 };

    /* The follow program:

    for(i = 1; i < 100; ++i)
        myStatement;

    */

    ParallelForStatement<
        AssignStatement<int,
            Variable<int, Context, &Context::i>,
            Literal<int, 1>
        >,
        LTComparisonStatement<
            Variable<int, Context, &Context::i>,
            Literal<int, 100>
        >,
        PreIncrStatement<int, Variable<int, Context, &Context::i>>,
        MyStatement
    > pf;

    pf(ctx);
}

int main()
{
    cout << "Serial for: " << endl;
    testSerialFor();

    cout << endl << "Parallel for: " << endl;
    testParallelFor();
}

