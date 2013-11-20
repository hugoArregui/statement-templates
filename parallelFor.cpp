/*
    Copyright (C) Daniel Gutson 2013
*/

// This code comes from Moirai project (http://moirai.googlecode.com)

#include "parallelFor.h"

void* _runThreadWrapper(void* arg)
{
    ThreadAdapter* const adapter = reinterpret_cast<ThreadAdapter*>(arg);
    adapter->run();
    delete adapter;
    return NULL;
}

