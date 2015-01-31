#pragma once

class IManager
{
public:
    IManager() = default;
    virtual ~IManager() = default;
    
    IManager(const IManager & ref) = delete;
	IManager(IManager && ref) = delete;
	IManager operator=(const IManager & ref) = delete;
	IManager operator=(IManager && ref) = delete;
};
