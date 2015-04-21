// use this directive instead of old IFDEF combination
#pragma once

// C++ header
#include <string>

// C header -> must be used <c***> version, not <***.h>
#include <cmath>

enum KeyType
{
    // do not set your own values if it is not necessary
    // did you remember that unions start from 0?
    KT_First,
    KT_Second
}

enum class RenderType
{
    Forward,
    Deferred
}

struct SomeStructure
{
    SomeStructure()         // small constructor
        : myVariable2(1.0f)
    {
    }
    
    void Func() const;
    int * Func2(float * p1, const std::string & p2);
    
    int myVariable1 = 0;    // may be initialized
    float myVariable2;      // or not
};

class ClassA
{
    // every member on different line
    // use camelCase notation from small letter
    int a1;
    int a2;
    int * a3;
    
public:
    ClassA()
        : a1(1)
        , a2(2)
        , a3(nullptr) // nullptr's everywhere, deprecate using of NULL
    {
    }
    
    virtual ~ClassA() {}
    virtual void PureVirtualFunc() = 0;
};

class ClassB : public ClassA // write type of inheritance everytime
{
public:
    void PureVirtualFunc() override {}  // use override everywhere
};

// do not be afraid of templates, they are good guys
// but don't forget about constexpr
template<typename T, size_t E>
class SomeTemplate
{
};

// but try to avoid templates like
template< template<...>, typename T, typename ... Args>
class Ouch {};

void Function()
{
    // some local function or method implementation
    // use camelCase from small letter here
    int a = 0;
    std::vector<int> inits = {1, 2, 4, 12};
    for (const auto & elem : inits) // use ranged-for everytime if it possible
        std::cout << elem << " " << std::endl;
        
    if (a > 0 && inits[0])
    {
        // use standard algorithms instead of handmade
        std::for_each(inits.cbegin(), inits.cend(), [](const int & e){std::cout << e << std::endl;});
    }

    // avoid to use strange and pre-optimized expressions
    a = 4511 >> 10;
    // instead of
    a = 4511 / 1024;
    // second is clearly, compiler will do anything he wants
}

// NO RESTRICTIONS on line length
// NO RESTRICTIONS variable names (try to use English of course)
