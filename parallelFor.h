/*
    Copyright (C) Daniel Gutson 2013
*/

#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#include <assert.h>
#include <pthread.h>
#include <unistd.h> // for obtaining processors count
#include <list>
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

// ============================================================================

template <class Init, class Condition, class Incr, class Body>
class ParallelForStatement : public ForStatement<Init, Condition, Incr, Body>
{
};

template <class AssignLeft, class AssignRight, class CompLeft, class CompRight, class Incr, class Body>
class ParallelForStatement<
        AssignStatement<int, AssignLeft, AssignRight>,
        LTComparisonStatement<CompLeft, CompRight>,
        Incr,
        Body
      > : public StatementBase<void>
{
    typedef AssignStatement<int, AssignLeft, AssignRight> AssignType;
    typedef LTComparisonStatement<CompLeft, CompRight> CompType;

    // This matches var = *, returns var
    template <class Context, int (Context::*Var), class Right>
    static bool canGetIndexVarFromAssignment(const Context&, const AssignStatement<int, Variable<int, Context, Var>, Right>&, int (Context::*&V))
    {
        V = Var;
        return true;
    }

    // IndexVar recognition from assignment failed
    template <class Context, class T>
    static bool canGetIndexVarFromAssignment(const Context&, const T&, int (Context::*&))
    {
        return false;
    }

    // This matches var < *, returns var
    template <class Context, int (Context::*Var), class Right>
    static bool canGetIndexVarFromComparison(const Context&, const LTComparisonStatement<Variable<int, Context, Var>, Right>&, int (Context::*&V))
    {
        V = Var;
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
    static bool canPredictLowerBound(Ctx&, const AssignStatement<int, Left, Literal<int, Value>>&, int& lbound)
    {
        lbound = Value;
        cout << "< lbound literal " << lbound << endl;
        return true;
    }

    // This matches x = Variable
    template <class Left, class Ctx, int (Ctx::*Var)>
    static bool canPredictLowerBound(Ctx& ctx, const AssignStatement<int, Left, Variable<int, Ctx, Var>>& v, int& lbound)
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
    bool canPredictBounds(Ctx& ctx, int& lbound, int& ubound)
    {
        int (Ctx::*assignmentVar);
        int (Ctx::*compVar);

        CompType comp;
        AssignType assign;

        return
            canGetIndexVarFromAssignment(ctx, assign, assignmentVar) &&
            canGetIndexVarFromComparison(ctx, comp, compVar) &&
            assignmentVar == compVar &&
            canPredictLowerBound(ctx, assign, lbound) &&
            canPredictUpperBound(ctx, comp, ubound);
    }

    /* Given an integer variable k, the following class transforms an assignment of the type
         x = y
       to the form
         x = k

       For that, k has to exist in the context.
    */
    template <class Context, class Left, int (Context::*K)>
    struct TransformAssignment : public AssignStatement<int, Left, Variable<int, Context, K>>
    {
        using AssignStatement<int, Left, Variable<int, Context, K>>::operator();
    };

    /* Given an integer variable k, the following class transforms comparison of the type
         x < y
       to the form
         x < k

       For that, k has to exist in the context.
    */
    template <class Context, class Left, int (Context::*K)>
    struct TransformComparison : public LTComparisonStatement<Left, Variable<int, Context, K>>
    {
        using LTComparisonStatement<Left, Variable<int, Context, K>>::operator();
    };

    template <class Context>
    class ThreadAdapterImplementation : public ThreadAdapter
    {
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
            void operator=(const AugmentedContext&) = delete;
        };

        AugmentedContext context;    // a copy!

        virtual void run()
        {
            typedef TransformAssignment<AugmentedContext, AssignLeft, &AugmentedContext::newInitialValue> NewAssign;
            typedef TransformComparison<AugmentedContext, CompLeft, &AugmentedContext::newUpperBound> NewComp;

            ForStatement<NewAssign, NewComp, Incr, Body> serialFor;
            serialFor(context);
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
            assert(processorsCount > 0);
            const int iterationsPerThread = iterations / processorsCount;
            const int extraIterations = iterations % processorsCount;
            std::list<pthread_t> threads;
            pthread_t handle;
            int initial = lbound;

            for (int i = 0; i < processorsCount; ++i)
            {
                const int upper = initial + iterationsPerThread +
                                    (i == 0 ? extraIterations : 0);   // we add the extra iterations to the first thread
                pthread_create(&handle, nullptr, &_runThreadWrapper, new ThreadAdapterImplementation<T>(context, initial, upper));
                threads.push_back(handle);
                initial = upper + 1;
            }

            for (auto h : threads)
            {
                pthread_join(h, nullptr);
            }
        }
        else
        {
            // Do a serial for:
            ForStatement<AssignType, CompType, Incr, Body> serialFor;
            serialFor(context);
        }
    }    
};

#endif

