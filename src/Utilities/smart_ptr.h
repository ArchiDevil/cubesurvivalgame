#pragma once

#include "cRefcounted.h"

template<typename T>
class smart_ptr
{
public:
    smart_ptr(T * ptr = nullptr)
    {
        if (ptr != nullptr)
            ptr->addref();
    }

    smart_ptr(const smart_ptr & ref)
    {
        this->ptr = ref.ptr;
        if (ptr != nullptr)
            ptr->addref();
    }

    ~smart_ptr()
    {
        if (ptr != nullptr)
            ptr->release();
    }

    T * GetPtr()
    {
        return ptr;
    }

private:
    T * ptr;

};
