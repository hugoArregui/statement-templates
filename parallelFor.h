/*
    Copyright (C) Daniel Gutson 2013
*/

#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H


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
struct ParallelForStatement : StatementBase<void>
{
    Init init;
    Condition cond;
    Incr incr;
    Body body;

    // This matches x < Literal
    template <class Ctx, class Left, int Value>
    bool canPredictUpperBound(Ctx&, const LTComparissonStatement<Left, Literal<int, Value>>&, int& ubound)
    {
        ubound = Value;
        cout << "< ubound literal " << ubound << endl;
        return true;
    }

    // This matches x < Variable
    template <class Left, class Ctx, int (Ctx::*Var)>
    bool canPredictUpperBound(Ctx& ctx, const LTComparissonStatement<Left, Variable<int, Ctx, Var>>& v, int& ubound)
    {
        ubound = v(ctx);
        cout << "< ubound Variable " << ubound << endl;
        return true;
    }

    template <class Ctx, class T>
    bool canPredictUpperBound(Ctx&, const T&, int& ubound)
    {
        cout << "ubound Other" << endl;
        return false;
    }

    template <class T>
    void operator()(T& context)
    {
        int iterations;

        if (canPredictIterations(context, cond, iterations))
        {
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
private:
    struct ThreadAdapterImplementation
    {
        virtual void run() = 0;
        virtual ~ThreadAdapter()
        {}
    };
    
};

#endif

