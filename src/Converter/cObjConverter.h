#pragma once

#include "IConverter.h"
#include "misc.h"

class cObjConverter : public IConverter
{
public:
    virtual bool Convert(const std::string & input, const std::string & output) override;
};
