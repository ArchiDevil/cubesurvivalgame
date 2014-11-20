#pragma once

class IManager
{
public:
	IManager() {}
    ~IManager() {}
    
    IManager(const IManager & ref) = delete;
	IManager(IManager && ref) = delete;
	IManager operator=(const IManager & ref) = delete;
	IManager operator=(IManager && ref) = delete;
};
