#pragma once

#include <list>
#include <algorithm>

#include "IObserver.h"

class IObserver;

class IObservable
{
public:
	virtual void addObserver(IObserver * obs)
	{
		observers.push_back(obs);
	}

	virtual void removeObserver(IObserver * obs)
	{
		std::remove(observers.begin(), observers.end(), obs);
	}

	virtual void notifyObservers() = 0;		//user must implement this method

private:
	std::list<IObserver *> observers;
};