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
 * Authors:         Daniel Gutson (original idea and code)
 *                  Hugo Arregui (Cyclomatic Complexity)
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
#include "cyclomaticComplexity.h"
#include "statementsListBuilder.h"
#include "staticChecking.h"

struct Context
{
    int i;
    int x;
    int y;
    unsigned int r;
};

struct MyStatement : StatementBase<void>
{
    template <class T>
    void operator()(T& context)
    {
        cout << "i = " << context.i << endl;
    }
};

struct MyRStatement : StatementBase<void>
{
    template <class T>
    void operator()(T& context)
    {
        cout << "r = " << context.r << endl;
    }
};

static void testSerialFor()
{
    Context ctx { 0, 0, 0, 0 };

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
        StatementsListBuilder<
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
        >::Type
    > f;

    f(ctx);
    cout << "x: " << ctx.x << endl << "y: " << ctx.y << endl;
}

static void testParallelFor()
{
    Context ctx { 0, 0, 0, 0 };

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
        PostIncrStatement<int, Variable<int, Context, &Context::i>>,
        MyStatement
    > pf;

    pf(ctx);
}

static void testLoopUnroll()
{
    Context ctx { 0, 0, 0, 1 };

    UnrollFormTransform<
        ForStatement <
            AssignStatement<unsigned int,
                Variable<unsigned int, Context, &Context::r>,
                Literal<unsigned int, 1>
            >,
            LTComparisonStatement<
                Variable<unsigned int, Context, &Context::r>,
                Literal<unsigned int, 100>
            >,
            PostIncrStatement<unsigned int, Variable<unsigned int, Context, &Context::r> >,
            MyRStatement
        >
    >::Type uf;

    uf(ctx);
}

static void testCyclomaticComplexity() 
{
    cout << "cyclomatic complexity: " << CyclomaticComplexity<MyStatement>::value << std::endl;
    cout << "cyclomatic complexity: " << 
        CyclomaticComplexity<
            IfStatement<
                LTComparisonStatement<Variable<int, Context, &Context::i>, Literal<int, 100>>,
                MyStatement
            >
        >::value << std::endl;
    cout << "cyclomatic complexity: " << 
        CyclomaticComplexity<
                StatementsListBuilder<
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
                >::Type
        >::value << std::endl;
    cout << "cyclomatic complexity: " << 
        CyclomaticComplexity<
            IfStatement<
                LTComparisonStatement<Variable<int, Context, &Context::i>, Literal<int, 100>>,
                IfStatement<
                    LTComparisonStatement<Variable<int, Context, &Context::i>, Literal<int, 100>>,
                    MyStatement
                >
            >
        >::value << std::endl;
    cout << "cyclomatic complexity: " << 
        CyclomaticComplexity<
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
                StatementsListBuilder<
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
                        IfStatement<
                            LTComparisonStatement<Variable<int, Context, &Context::i>, Literal<int, 100>>,
                            MyStatement
                        >
                    >
                >::Type
            > 
        >::value << std::endl;
}

static void testStaticChecking()
{
    typedef
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
            StatementsListBuilder <
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
                    IfStatement<
                        LTComparisonStatement<Variable<int, Context, &Context::i>, Literal<int, 100>>,
                        MyStatement
                    >
                >
            >::Type
        >

        Program;

    StaticCheck<Program>::dummy();
}

int main()
{
    cout << "Serial for: " << endl;
    testSerialFor();

    cout << endl << "Parallel for: " << endl;
    testParallelFor();

    cout << endl << "Unrolled for: " << endl;
    testLoopUnroll();

    testCyclomaticComplexity();

    testStaticChecking();
}

