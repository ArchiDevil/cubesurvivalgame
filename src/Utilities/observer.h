#pragma once

#include <list>

template<typename T>
class observer;

template<typename T>
class notifier
{
public:
	notifier() {}
	~notifier() {}
	void subscribe(observer<T> * obs) {observers.push_back(obs);}
	void unsubscribe(observer<T> * obs) {observers.remove(obs);}
	virtual void notifyAll() = 0;
	
protected:
	std::list<observer<T>*> observers;
};

template<typename T>
class observer
{
public:
	observer() {}
	~observer() {}
	virtual bool handleEvent(const T & event) = 0;
	
protected:
	notifier<T> * cur_notifier;
};
