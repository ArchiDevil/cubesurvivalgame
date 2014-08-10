#pragma once

#include "notifier.h"

template<typename T>
class notifier;

template<typename T>
class observer
{
public:
	observer() {}
	~observer() {}
	
	virtual bool handleEvent(const T & event) = 0;
	
protected:
	notifier<T> * cur_notifier;	//may be observe more than one notifier?
	
};