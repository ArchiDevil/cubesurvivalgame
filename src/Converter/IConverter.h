#pragma once

#include "LIMSaver.h"
#include "misc.h"

class IConverter
{
public:
	virtual ~IConverter() {}
	virtual bool Convert(const std::wstring & in, const std::wstring & out) = 0;
};

