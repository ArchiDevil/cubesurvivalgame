#pragma once

#include <fstream>
#include <string>
#include <map>

class IniWorker
{
public:
    bool            Initialize(const std::string & FileName);
    int             GetInteger(const std::string & Parameter) const;
    float           GetFloat(const std::string & Parameter) const;
    bool            GetBoolean(const std::string & Parameter) const;
    std::string     GetString(const std::string & Parameter) const;
    std::wstring    GetWString(const std::string & Parameter) const;

private:
    void            CollectKeys(std::ifstream & stream);
    std::string     GetKey(const std::string & Key) const;

    std::map<std::string, std::string> data;
};
