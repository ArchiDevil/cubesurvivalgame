#pragma once

#include "IConverter.h"
#include "misc.h"

class cXConverter : public IConverter
{
public:
	virtual bool Convert( const std::wstring & in, const std::wstring & out );
};

