/**
 * \file  cyclomaticComplexity.h
 * \brief Cyclomatic Complexity sample.
 *
 * Statements-templates: Extending expression templates to statements and flow control
 * <http://statement-templates.googlecode.com/>
 * Copyright (C) 2013 Daniel Gutson, Taller Technologies
 *
 * This file is part of the Statement-templates project.
 *
 * Author:        Hugo Arregui 
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

#include "utils.h"

// Based on: http://users.csc.calpoly.edu/~jdalbey/206/Lectures/BasisPathTutorial/index.html
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

template <class Head>
struct CyclomaticComplexity<StatementsList<Head, NOP>> 
{
    static const int value = CyclomaticComplexity<Head>::value;
};

template <class Head, class Tail>
struct CyclomaticComplexity<StatementsList<Head, Tail>> 
{
    static const int value = Max<CyclomaticComplexity<Head>::value, CyclomaticComplexity<Tail>::value>::value;
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
