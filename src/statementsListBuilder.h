/**
 * \file  statementsListBuilder.h
 * \brief syntax sugar for statements list generation
 *
 * Statements-templates: Extending expression templates to statements and flow control
 * <http://statement-templates.googlecode.com/>
 * Copyright (C) 2013 Daniel Gutson, Taller Technologies
 *
 * This file is part of the Statement-templates project.
 *
 * Author:         Hugo Arregui
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

#ifndef STATEMENTS_LIST_BUILDER_H
#define STATEMENTS_LIST_BUILDER_H

template <class ... Statements>
struct StatementsListBuilder;

template <class Last>
struct StatementsListBuilder<Last> : Last
{
    typedef StatementsList<Last, NIL> Type;
};

template <class Head, class... Tail>
struct StatementsListBuilder<Head, Tail...> : StatementsListBuilder<Tail...>
{
    typedef StatementsList<Head, typename StatementsListBuilder<Tail...>::Type> Type;
};

#endif

