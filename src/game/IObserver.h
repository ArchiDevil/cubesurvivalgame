#pragma once

#include <list>

#include "events.h"
#include "INotifer.h"

class IObservable;

class IObserver
{
public:
	~IObserver()
	{
		for (auto it = subjects.begin(); it != subjects.end(); it++)
		{
			(*it)->removeObserver(this);
		}
	}

	virtual void ProcessEvent(GameEvent ev, void * par) = 0;

private:
	std::list<IObservable*> subjects;
};