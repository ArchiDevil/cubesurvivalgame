#pragma once

#include "IConverter.h"

class cBlkConverter : public IConverter
{
public:
	virtual bool Convert( const std::wstring & in, const std::wstring & out ) override;

};

