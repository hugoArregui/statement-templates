/*
    Copyright (C) Daniel Gutson 2013
*/

// This is the proposed syntax. Of course this doesn't compile in current C++.

class C
{
    operator for()(context ctx, void statement init, bool statement condition, void statement incr, void statement body)
    {
        for(init(ctx); condition(ctx); incr(ctx))
            body(ctx);
    }
};

class P
{
    operator for()(context ctx, void statement init, bool statement condition, void statement incr, void statement body)
    {
        for(init(ctx); condition(ctx); incr(ctx))
            body(ctx);
    }
};

template <class ForLoopType>
void f()
{
    ForLoopType f;
    f.for(int i=0; i < 10; ++i)
    {
        cout << "Hello " << i << endl;
    }
}

