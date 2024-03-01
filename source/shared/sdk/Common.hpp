#include "Common.h"
#include "tlhelp32.h"
#include "Psapi.h"

#pragma once



bool SharedUtil::IsGTAProcess()
{
    SString strLaunchPathFilename = GetLaunchPathFilename();
    return strLaunchPathFilename.EndsWithI("SanAndreas.exe");
}


SString SharedUtil::GetParentProcessPathFilename(int pid)
{
    HANDLE          hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W pe = { sizeof(PROCESSENTRY32W) };
    if (Process32FirstW(hSnapshot, &pe))
    {
        do
        {
            if (pe.th32ProcessID == pid)
            {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ParentProcessID);
                if (hProcess)
                {
                    WCHAR szModuleName[MAX_PATH * 2] = { 0 };
                    GetModuleFileNameExW(hProcess, nullptr, szModuleName, NUMELMS(szModuleName));
                    CloseHandle(hProcess);
                    SString strModuleName = ToUTF8(szModuleName);
                    if (FileExists(strModuleName))
                    {
                        CloseHandle(hSnapshot);
                        if (IsShortPathName(strModuleName))
                            return GetSystemLongPathName(strModuleName);
                        return strModuleName;
                    }
                }
            }
        } while (Process32NextW(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
    return "";
}

SString SharedUtil::CalculePath(const SString& strPath)
{
    static SString strInstallRoot;

    if (strInstallRoot.empty())
    {
        if (IsGTAProcess())
        {
            strInstallRoot = ExtractPath(GetParentProcessPathFilename(GetCurrentProcessId()));
        }
    }

    return PathJoin(strInstallRoot, strPath);
}
