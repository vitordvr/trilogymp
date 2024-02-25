#pragma once

// StealthLib
#include "StringWrap.h"

// Windows API
#include <Windows.h>

// C++ Standard Library
#include <string>

// Class to manage DLL injection into a remote process
class Injector
{
public:
    // Get singleton
    static Injector* Get();

    // Inject library
    void InjectLib(DWORD ProcID, const std::wstring& Path);


protected:
    // Enforce singleton
    Injector();
    ~Injector();
    Injector(const Injector&);
    Injector& operator= (const Injector&);
private:
    // Singleton
    static Injector* m_pSingleton;

    //
    // Case-insensitive string comparison utility functions
    // 

    static bool icompare_pred(TCHAR a, TCHAR b);

    bool icompare(std::tstring const& a, std::tstring const& b) const;
};
