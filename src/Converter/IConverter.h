#pragma once

#include "LIMSaver.h"
#include "misc.h"

class IConverter
{
public:
    virtual ~IConverter() {}
    virtual bool Convert(const std::string & in, const std::string & out) = 0;
};
