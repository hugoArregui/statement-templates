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

template <unsigned int FROM, unsigned int TO, class Incr, class Body>
struct UnrollLoop;

template <int END, unsigned int FROM, unsigned int TO, class Incr, class Body>
struct UnrollStep {
};

template <unsigned int FROM, unsigned int TO, class Incr, class Body>
struct UnrollStep<1, FROM, TO, Incr, Body> {
    typedef StatementsList<Body, StatementsList<Incr, typename UnrollLoop<FROM+1, TO, Incr, Body>::Type> > Type;
};

template <unsigned int FROM, unsigned int TO, class Incr, class Body>
struct UnrollStep<0, FROM, TO, Incr, Body> {
    typedef StatementsList<Body, NullList> Type;
};

template <unsigned int FROM, unsigned int TO, class Incr, class Body>
struct UnrollLoop {
    typedef typename UnrollStep<FROM < (TO - 1), FROM, TO, Incr, Body>::Type Type;
};

template <unsigned int FROM, unsigned int TO, class Init, class Incr, class Body>
struct Unroll {
    typedef StatementsList<Init, typename UnrollLoop<FROM, TO, Incr, Body>::Type> Type;
};

template <class Init, class Condition, class Incr, class Body>
struct UnrolledForStatement : public ForStatement<Init, Condition, Incr, Body>
{
    // Default to basic for
};

template <class S>
struct Transform 
{
    typedef S Type;
};

template <class Var, unsigned int FROM, unsigned int TO, class Body>
struct Transform<
            ForStatement <
                AssignStatement<unsigned int, Var, Literal<unsigned int, FROM> >,
                LTComparisonStatement<Var, Literal<unsigned int, TO> >,
                PostIncrStatement<unsigned int, Var>,
                Body
            > 
        >
{
    typedef AssignStatement<unsigned int, Var, Literal<unsigned int, FROM> > Init;
    typedef typename Unroll<FROM, TO, Init, PostIncrStatement<unsigned int, Var>, Body>::Type Type;
};

#endif

