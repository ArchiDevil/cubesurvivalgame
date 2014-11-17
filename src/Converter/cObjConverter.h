#pragma once

#include "IConverter.h"
#include "misc.h"

class cObjConverter: public IConverter
{
public:
	virtual bool Convert(const std::wstring & input, const std::wstring & output);

};
