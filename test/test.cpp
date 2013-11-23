/**
 * \file  test.cpp
 * \brief Basic tests for demonstration.
 *
 * Statements-templates: Extending expression templates to statements and flow control
 * <http://statement-templates.googlecode.com/>
 * Copyright (C) 2013 Daniel Gutson, Taller Technologies
 *
 * This file is part of the Statement-templates project.
 *
 * Author:         Daniel Gutson
 *
 * Statement-tamplets is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Statement-tamplets is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Statement-tamplets.  If not, see <http://www.gnu.org/licenses/>.
 *
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

void testCyclomaticComplexity() {
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
            IfStatement<
                LTComparisonStatement<Variable<int, Context, &Context::i>, Literal<int, 100>>,
                MyStatement
            >
        >
    > f;

    cout << "cyclomatic complexity: " << f.complexity << endl;
}

int main()
{
    cout << "Serial for: " << endl;
    testSerialFor();

    cout << endl << "Parallel for: " << endl;
    testParallelFor();

    testCyclomaticComplexity();
}

