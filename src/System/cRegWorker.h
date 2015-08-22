#pragma once

#include <string>
#include <Windows.h>

//stolen from windows.h :3
enum Root
{
    HKEY_LOCAL_MACHINE_R = (((LONG)0x80000002)),
    HKEY_CURRENT_USER_R = (((LONG)0x80000001)),
    HKEY_CLASSES_ROOT_R = (((LONG)0x80000000)),
    HKEY_USERS_R = (((LONG)0x80000003)),
    HKEY_CURRENT_CONFIG_R = (((LONG)0x80000005)),
};

class cRegWorker
{
public:
    static std::wstring GetString(Root root, std::wstring path, std::wstring parameter);
    static int GetInteger(Root root, std::wstring path, std::wstring parameter);

    void SetString(Root root, const std::wstring & path, const std::wstring & param, const std::wstring & val);
    void SetInteger(Root rootm, const std::wstring & path, const std::wstring & param, int val);
};
