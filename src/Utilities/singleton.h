#pragma once

template<typename T> 
class singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance; // у класса T есть конструктор по умолчанию
		return theSingleInstance;
	}
};