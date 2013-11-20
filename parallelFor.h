/*
    Copyright (C) Daniel Gutson 2013
*/

#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#include "basicStatements.h"

// This is obtained from Moirai project (http://moirai.googlecode.com) ========
// and is just for the proof of concept. Ignore this snippet.
struct ThreadAdapter
{
    virtual void run() = 0;
    virtual ~ThreadAdapter()
    {}
};

extern "C" void* _runThreadWrapper(void* arg);

void* _runThreadWrapper(void* arg)
{
    ThreadAdapter* const adapter = reinterpret_cast<ThreadAdapter*>(arg);
    adapter->run();
    delete adapter;
    return nullptr;
}
// ============================================================================

template <class Init, class Condition, class Incr, class Body>
class ParallelForStatement : ForStatement<Init, Condition, Incr, Body>
{
};

template <class AssignLeft, class AssignRight, class CompLeft, CompRight, class Incr, class Body>
class ParallelForStatement : StatementBase<void>
{
    Init init;
    Condition cond;
    Incr incr;
    Body body;

    // This matches var = *, returns var
    template <class Context, int (Context::*Variable), class Right>
    static bool canGetIndexVarFromAssignment(const Context&, const AssignStatement<Variable<int, Context, Variable>, Right>&, int (Context::*&V))
    {
        V = Variable;
        return true;
    }

    // IndexVar recognition from assignment failed
    template <class Context, class T>
    static bool canGetIndexVarFromAssignment(const Context&, const T&, int (Context::*&))
    {
        return false;
    }

    // This matches var < *, returns var
    template <class Context, int (Context::*Variable), class Right>
    static bool canGetIndexVarFromComparison(const Context&, const LTComparison<Variable<int, Context, Variable>, Right>&, int (Context::*&V))
    {
        V = Variable;
        return true;
    }

    // IndexVar recognition from comparison failed
    template <class Context, class T>
    static bool canGetIndexVarFromComparison(const Context&, const T&, int (Context::*&))
    {
        return false;
    }

    // This matches x < Literal
    template <class Ctx, class Left, int Value>
    static bool canPredictUpperBound(Ctx&, const LTComparisonStatement<Left, Literal<int, Value>>&, int& ubound)
    {
        ubound = Value;
        cout << "< ubound literal " << ubound << endl;
        return true;
    }

    // This matches x < Variable
    template <class Left, class Ctx, int (Ctx::*Var)>
    static bool canPredictUpperBound(Ctx& ctx, const LTComparisonStatement<Left, Variable<int, Ctx, Var>>& v, int& ubound)
    {
        ubound = v(ctx);
        cout << "< ubound Variable " << ubound << endl;
        return true;
    }

    // Upper bound prediction failure
    template <class Ctx, class T>
    static bool canPredictUpperBound(Ctx&, const T&, int&)
    {
        cout << "ubound Other" << endl;
        return false;
    }

    // This matches x = Literal
    template <class Ctx, class Left, int Value>
    static bool canPredictLowerBound(Ctx&, const AssignStatement<Left, Literal<int, Value>>&, int& lbound)
    {
        lbound = Value;
        cout << "< lbound literal " << lbound << endl;
        return true;
    }

    // This matches x = Variable
    template <class Left, class Ctx, int (Ctx::*Var)>
    static bool canPredictLowerBound(Ctx& ctx, const AssignStatement<Left, Variable<int, Ctx, Var>>& v, int& lbound)
    {
        lbound = v(ctx);
        cout << "< lbound Variable " << lbound << endl;
        return true;
    }

    // Lower bound prediction failure
    template <class Ctx, class T>
    static bool canPredictLowerBound(Ctx&, const T&, int&)
    {
        cout << "lbound Other" << endl;
        return false;
    }

    template <class Ctx>
    bool canPredictBounds(Ctx& ctx, int& lbound, int& ubound, int (Ctx::*& indexVar))
    {
        int (Ctx::*assignmentVar);

        return
            canGetIndexVarFromAssignment(ctx, init, assignmentVar) &&
            canGetIndexVarFromComparison(ctx, init, indexVar) &&
            assignmentVar == indexVar &&
            canPredictLowerBound(ctx, init, lbound) &&
            canPredictUpperBound(ctx, cond, ubound);
    }

    /* Given an integer variable k, the following class transforms an assignment of the type
         x = y
       to the form
         x = k

       For that, k has to exist in the context.
    */
    template <class Context, class Left, int (Context::*K)>
    struct TransformAssignment : public AssignStatement<int, Left, Variable<int, K>>
    {
    };

    /* Given an integer variable k, the following class transforms comparison of the type
         x < y
       to the form
         x < k

       For that, k has to exist in the context.
    */
    template <class Context, class Left, int (Context::*K)>
    struct TransformComparison : public ComparisonStatement<Left, Variable<int, K>>
    {
    };

    template <class Context>
    class ThreadAdapterImplementation
    {
        Body body;

        struct AugmentedContext : Context
        {
            AugmentedContext(const Context& base, int newInitialValue, int newUpperBound)
                : Context(base),
                  newInitialValue(newInitialValue),
                  newUpperBound(newUpperBound)
            {}

            int newInitialValue;
            int newUpperBound;

            AugmentedContext() = delete;
        };

        AugmentedContext context;    // a copy!

        virtual void run()
        {
            TransformComparison<AugmentedContext, 
            body(context);
        }

    public:
        ThreadAdapterImplementation(const Context& ctx, int initial, int upper) : context(ctx, initial, upper) {}
    };

public:
    template <class T>
    void operator()(T& context)
    {
        int lbound;
        int ubound;

        if (canPredictBounds(context, lbound, ubound))
        {
            const int iterations = ubound - lbound;
            const int processorsCount = sysconf(_SC_NPROCESSORS_ONLN);
            list<phtread_t> threads;
        }
        else
        {
            // Do a serial for:
            ForStatement<Init, Condition, Incr, Body> serialFor;
            serialFor(context);
        }
    }    
};

#endif

