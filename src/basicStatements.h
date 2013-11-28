/**
 * \file  basicStatements.h
 * \brief Basic statements implementation
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

#ifndef BASIC_STATEMENTS_H
#define BASIC_STATEMENTS_H

#include <iostream>
using namespace std;

template <class RetType>
struct StatementBase
{
    typedef RetType ReturnType;
};

struct NOP : StatementBase<void>
{
    template <class T>
    void operator()(T& /*context*/)
    {
    }
};

template <class RetType, RetType Value>
struct Literal : StatementBase<RetType>
{
    template <class T>
    RetType operator()(T& /*context*/)
    {
        return Value;
    }
};

template <class RetType, class Ctx, RetType (Ctx::*Var)>
struct Variable : StatementBase<RetType>
{
    template <class T>
    RetType& operator()(T& context)
    {
        return (context.*Var);
    }
};

template <class RetType, class Left, class Right>
struct AddStatement : StatementBase<RetType>
{
    Left leftStatement;
    Right rightStatement;
    
    template <class T>
    RetType operator()(T& context)
    {
        return leftStatement(context) + rightStatement(context);
    }
};

template <class RetType, class LValue, class RValue>
struct AssignStatement : StatementBase<RetType>
{
    LValue lvalue;
    RValue rvalue;

    template <class T>
    RetType& operator()(T& context)
    {
        return lvalue(context) = rvalue(context);
    }
};

template <class RetType, class LValue>
struct PreIncrStatement : StatementBase<RetType>
{
    LValue lvalue;

    template <class T>
    RetType& operator()(T& context)
    {
        return ++lvalue(context);
    }
};

template <class RetType, class LValue>
struct PostIncrStatement : StatementBase<RetType>
{
    LValue lvalue;

    template <class T>
    RetType operator()(T& context)
    {
        return lvalue(context)++;
    }
};

template <class RetType, class LValue, class RValue>
struct AddAssignStatement : StatementBase<RetType>
{
    LValue lvalue;
    RValue rvalue;

    template <class T>
    RetType operator()(T& context)
    {
        return lvalue(context) += rvalue(context);
    }
};

template <class Left, class Right>
struct ComparisonStatement: StatementBase<bool>
{
    Left left;
    Right right;
};

template <class Left, class Right>
struct LTComparisonStatement: ComparisonStatement<Left, Right>
{
    Left left;
    Right right;

    template <class T>
    bool operator()(T& context)
    {
        return left(context) < right(context);
    }
};

struct NIL {};

template <class Head, class Tail>
struct StatementsList : public Tail
{
    typedef typename Tail::ReturnType ReturnType;

    Head head;

    template <class T>
    ReturnType operator()(T& context)
    {
        head(context);
        return Tail::operator()(context);
    }
};

template <class Head>
struct StatementsList<Head, NIL>  : StatementBase<void>
{
    typedef typename Head::ReturnType ReturnType;
    Head head;

    template <class T>
    ReturnType operator()(T& context)
    {
        return head(context);
    }
};

template <class Condition, class TruePart, class ElsePart = NOP>
struct IfStatement : StatementBase<void>
{
    Condition cond;
    TruePart tp;
    ElsePart ep;

    template <class T>
    void operator()(T& context)
    {
        if (cond(context))
            tp(context);
        else
            ep(context);
    }
};

template <class Init, class Condition, class Incr, class Body>
struct ForStatement : StatementBase<void>
{
    Init init;
    Condition cond;
    Incr incr;
    Body body;

    template <class T>
    void operator()(T& context)
    {
        for(init(context); cond(context); incr(context))
            body(context);
    }
};

#endif

