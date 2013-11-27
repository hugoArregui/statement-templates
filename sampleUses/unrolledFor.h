/**
 * \file  unrolledFor.h
 * \brief Sample for-loop unrolling.
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

#ifndef UNROLLED_FOR_H
#define UNROLLED_FOR_H

#include "basicStatements.h"
#include "utils.h"

template <unsigned int FROM, unsigned int TO, class Init, class Incr, class Body>
struct Unroll 
{
    template <unsigned int START>
    struct UnrollLoop;

    template <bool END, unsigned int START>
    struct UnrollStep 
    {};

    template <unsigned int START>
    struct UnrollStep<true, START> 
    {
        typedef StatementsList<Body, StatementsList<Incr, typename UnrollLoop<START+1>::Type> > Type;
    };

    template <unsigned int START>
    struct UnrollStep<false, START> 
    {
        typedef StatementsList<Body, NIL> Type;
    };

    template <unsigned int START>
    struct UnrollLoop 
    {
        typedef typename UnrollStep<START < (TO - 1), START>::Type Type;
    };

    typedef typename If<(FROM < TO), StatementsList<Init, typename UnrollLoop<FROM>::Type>, NOP>::Type Type;
};

template <class S>
struct BaseUnrollFormTransform
{
    typedef S Type;
};

template <class Var, unsigned int FROM, unsigned int TO, class Incr, class Body>
struct BaseUnrollFormTransform <
            ForStatement <
                AssignStatement<unsigned int, Var, Literal<unsigned int, FROM> >,
                LTComparisonStatement<Var, Literal<unsigned int, TO> >,
                Incr,
                Body
            > 
        >
{
    typedef AssignStatement<unsigned int, Var, Literal<unsigned int, FROM> > Init;
    typedef typename Unroll<FROM, TO, Init, Incr, Body>::Type Type;
};

template <class S>
struct UnrollFormTransform : BaseUnrollFormTransform<S>
{
};

template <class Var, class Assign, class Comp, class Body>
struct UnrollFormTransform <
            ForStatement < Assign, Comp, PreIncrStatement<unsigned int, Var>, Body > 
        > : BaseUnrollFormTransform <
                ForStatement < Assign, Comp, PreIncrStatement<unsigned int, Var>, Body > 
            >
{
};

template <class Var, class Assign, class Comp, class Body>
struct UnrollFormTransform <
            ForStatement < Assign, Comp, PostIncrStatement<unsigned int, Var>, Body > 
        > : BaseUnrollFormTransform <
                ForStatement < Assign, Comp, PostIncrStatement<unsigned int, Var>, Body > 
            >
{
};

#endif

