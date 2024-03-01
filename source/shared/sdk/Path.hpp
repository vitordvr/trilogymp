#pragma once
#include "Path.h"
#include "Defines.h"

#include "WString.h"
#include "IntTypes.h"
#include "Windows.h"
#include "shellapi.h"
#include "shlobj.h"
#include <shlwapi.h>
#include <algorithm>

bool SharedUtil::FileExists(const SString& strFilename)
{
    DWORD dwAtr = GetFileAttributes(strFilename);
    if (dwAtr == INVALID_FILE_ATTRIBUTES)
        return false;
    return ((dwAtr & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

bool SharedUtil::DirectoryExists(const SString& strPath)
{
    DWORD dwAtr = GetFileAttributes(strPath);
    if (dwAtr == INVALID_FILE_ATTRIBUTES)
        return false;
    return ((dwAtr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}



bool SharedUtil::MkDir(const SString& strInPath, bool bTree)
{
    SString strPath = PathConform(strInPath);
    MakeSureDirExists(strPath + PATH_SEPERATOR);
    return DirectoryExists(strPath);
}


void SharedUtil::MakeSureDirExists(const SString& strPath)
{
    std::vector<SString> parts;
    PathConform(strPath).Split(PATH_SEPERATOR, parts);

    // Find first dir that already exists
    int idx = parts.size() - 1;
    for (; idx >= 0; idx--)
    {
        SString strTemp = SString::Join(PATH_SEPERATOR, parts, 0, idx);
        if (DirectoryExists(strTemp))
            break;
    }

    // Make non existing dirs only
    idx++;
    for (; idx < (int)parts.size(); idx++)
    {
        SString strTemp = SString::Join(PATH_SEPERATOR, parts, 0, idx);
        // Call mkdir on this path
        File::Mkdir(strTemp);
    }
}


SString SharedUtil::PathConform(const SString& strPath)
{
    // Make slashes the right way and remove duplicates, except for UNC type indicators
    SString strTemp = strPath.Replace("/", PATH_SEPERATOR);

    // Remove slash duplicates
    size_t iFirstDoubleSlash = strTemp.find(PATH_SEPERATOR PATH_SEPERATOR);
    if (iFirstDoubleSlash == std::string::npos)
        return strTemp;            // No duplicates present

    // If first double slash is not at the start, then treat as a normal duplicate if:
    //      1. It is not preceeded by a colon, or
    //      2. Another single slash is before it
    if (iFirstDoubleSlash > 0)
    {
        if (iFirstDoubleSlash == 2 && strTemp[1] == ':')
        {
            // Replace all duplicate slashes
            return strTemp.Replace(PATH_SEPERATOR PATH_SEPERATOR, PATH_SEPERATOR, true);
        }

        if (strTemp.SubStr(iFirstDoubleSlash - 1, 1) != ":" || strTemp.find(PATH_SEPERATOR) < iFirstDoubleSlash)
        {
            // Replace all duplicate slashes
            return strTemp.Replace(PATH_SEPERATOR PATH_SEPERATOR, PATH_SEPERATOR, true);
        }
    }

    return strTemp.Left(iFirstDoubleSlash + 1) + strTemp.SubStr(iFirstDoubleSlash + 1).Replace(PATH_SEPERATOR PATH_SEPERATOR, PATH_SEPERATOR, true);
}


SString SharedUtil::PathJoin(const SString& str1, const SString& str2)
{
    return PathConform(str1 + PATH_SEPERATOR + str2);
}

SString SharedUtil::PathJoin(const SString& str1, const SString& str2, const SString& str3, const SString& str4, const SString& str5)
{
    SString strResult = str1 + PATH_SEPERATOR + str2 + PATH_SEPERATOR + str3;
    if (str4.length())
        strResult += PATH_SEPERATOR + str4;
    if (str5.length())
        strResult += PATH_SEPERATOR + str5;
    return PathConform(strResult);
}


SString SharedUtil::PathMakeRelative(const SString& strInBasePath, const SString& strInAbsPath)
{
    SString strBasePath = PathConform(strInBasePath);
    SString strAbsPath = PathConform(strInAbsPath);
    if (strAbsPath.BeginsWithI(strBasePath))
    {
        return strAbsPath.SubStr(strBasePath.length()).TrimStart(PATH_SEPERATOR);
    }
    return strAbsPath;
}


void SharedUtil::ExtractFilename(const SString& strInPathFilename, SString* strPath, SString* strFilename)
{
    const SString strPathFilename = PathConform(strInPathFilename);
    if (!strPathFilename.Split(PATH_SEPERATOR, strPath, strFilename, -1))
        if (strFilename)
            *strFilename = strPathFilename;
}

bool SharedUtil::ExtractExtension(const SString& strFilename, SString* strMain, SString* strExt)
{
    return strFilename.Split(".", strMain, strExt, -1);
}


SString SharedUtil::ExtractPath(const SString& strPathFilename)
{
    SString strPath;
    ExtractFilename(strPathFilename, &strPath, NULL);
    return strPath;
}


SString SharedUtil::ExtractExtension(const SString& strPathFilename)
{
    SString strExt;
    ExtractExtension(strPathFilename, NULL, &strExt);
    return strExt;
}


SString SharedUtil::ExtractFilename(const SString& strPathFilename)
{
    SString strFilename;
    ExtractFilename(strPathFilename, NULL, &strFilename);
    return strFilename;
}


SString SharedUtil::ExtractBeforeExtension(const SString& strPathFilename)
{
    SString strMain;
    ExtractExtension(strPathFilename, &strMain, NULL);
    return strMain;
}

int SharedUtil::File::Mkdir(const char* szPath, int iMode)
{
    return _wmkdir(FromUTF8(szPath));
}

WString SharedUtil::FromUTF8(const SString& strPath)
{
    const char* szSrc = strPath;
    uint        cCharacters = strlen(szSrc) + 1;
    uint        cbUnicode = cCharacters * 4;
    wchar_t* Dest = (wchar_t*)alloca(cbUnicode);

    if (MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, Dest, (int)cbUnicode) == 0)
    {
        return WString();
    }
    else
    {
        return Dest;
    }
}


// gta_sa.exe
SString SharedUtil::GetLaunchFilename()
{
    return ExtractFilename(GetLaunchPathFilename());
}


SString SharedUtil::GetLaunchPath()
{
    return ExtractPath(GetLaunchPathFilename());
}


SString SharedUtil::GetLaunchPathFilename()
{
    static SString strLaunchPathFilename;
    if (strLaunchPathFilename.empty())
    {
        wchar_t szBuffer[2048];
        GetModuleFileNameW(NULL, szBuffer, NUMELMS(szBuffer) - 1);

        wchar_t fullPath[MAX_PATH];
        GetFullPathNameW(szBuffer, MAX_PATH, fullPath, nullptr);

        strLaunchPathFilename = ToUTF8(fullPath);
    }
    return strLaunchPathFilename;
}


SString SharedUtil::ToUTF8(const WString& strPath)
{
    const wchar_t* pszW = strPath;
    uint           cCharacters = wcslen(pszW) + 1;
    uint           cbAnsi = cCharacters * 6;
    char* pData = (char*)alloca(cbAnsi);

    if (0 == WideCharToMultiByte(CP_UTF8, 0, pszW, cCharacters, pData, cbAnsi, NULL, NULL))
    {
        return "";
    }
    return pData;
}

bool SharedUtil::IsShortPathName(const char* szPath)
{
    return strchr(szPath, '~') != NULL;
}

bool SharedUtil::IsShortPathName(const wchar_t* szPath)
{
    return wcschr(szPath, '~') != NULL;
}

SString SharedUtil::GetSystemLongPathName(const SString& strPath)
{
    wchar_t szBuffer[32000];
    szBuffer[0] = 0;
    if (!GetLongPathNameW(FromUTF8(strPath), szBuffer, NUMELMS(szBuffer) - 1))
        return strPath;
    return ToUTF8(szBuffer);
}

SString SharedUtil::GetSystemDllDirectory()
{
    wchar_t szResult[1024] = L"";
    GetDllDirectoryW(NUMELMS(szResult), szResult);
    if (IsShortPathName(szResult))
        return GetSystemLongPathName(ToUTF8(szResult));
    return ToUTF8(szResult);
}

SString SharedUtil::GetSystemCurrentDirectory()
{
    wchar_t szResult[1024] = L"";
    GetCurrentDirectoryW(NUMELMS(szResult), szResult);
    if (IsShortPathName(szResult))
        return GetSystemLongPathName(ToUTF8(szResult));
    return ToUTF8(szResult);
}
