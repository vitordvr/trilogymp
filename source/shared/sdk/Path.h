#pragma once

#include <vector>
#include "IntTypes.h"
#include "SString.h"
#include "WString.h"


namespace SharedUtil {
    bool FileExists(const SString& strFilename);
    bool DirectoryExists(const SString& strPath);
    void MakeSureDirExists(const SString& strPath);

    SString PathConform(const SString& strInPath);
    SString PathJoin(const SString& str1, const SString& str2);
    SString PathJoin(const SString& str1, const SString& str2, const SString& str3, const SString& str4 = "", const SString& str5 = "");
    SString PathMakeRelative(const SString& strInBasePath, const SString& strInAbsPath);
    void    ExtractFilename(const SString& strPathFilename, SString* strPath, SString* strFilename);
    bool    ExtractExtension(const SString& strFilename, SString* strRest, SString* strExt);
    SString ExtractPath(const SString& strPathFilename);
    SString ExtractFilename(const SString& strPathFilename);
    SString ExtractExtension(const SString& strPathFilename);
    SString ExtractBeforeExtension(const SString& strPathFilename);

    bool                 MkDir(const SString& strInPath, bool bTree = true);

    WString FromUTF8(const SString& strPath);
    SString ToUTF8(const WString& strPath);

    SString GetLaunchPathFilename();
    SString GetLaunchPath();
    SString GetLaunchFilename();

    namespace File
    {
        int   Mkdir(const char* szPath, int iMode = 0775);
    }
}
