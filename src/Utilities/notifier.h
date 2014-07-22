#pragma once

#include <list>

#include "observer.h"

template<typename T>
class observer;

template<typename T>
class notifier
{
public:
	notifier() {}
	~notifier() {}
	
	void subscribe(observer<T> * obs)
	{
		observers.push_back(obs);
	}
	
	void de_subscribe(observer<T> * obs)
	{
		std::remove(observers.begin(), observers.end(), obs);
	}
	
	virtual void notifyAll() = 0;
	
protected:
	std::list<observer<T> *> observers;
	
};