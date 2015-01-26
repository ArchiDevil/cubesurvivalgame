#pragma once

#include "IConverter.h"
#include "misc.h"

class cXConverter : public IConverter
{
public:
    virtual bool Convert(const std::string & in, const std::string & out) override;
};
