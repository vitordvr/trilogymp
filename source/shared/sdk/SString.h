#pragma once
#pragma once

#include <string>
#include <vector>
#include <cstdarg>
#include "IntTypes.h"

#ifdef WIN32
#ifndef va_copy
#define va_copy(dest, orig) (dest) = (orig)
#endif
#endif

class SString : public std::string
{
public:
    // Constructors
    SString() : std::string() {}

    SString(const char* szText) : std::string(szText ? szText : "") {}

    explicit SString(const char* szFormat, ...) : std::string()
    {
        if (szFormat)
        {
            va_list vl;

            va_start(vl, szFormat);
            vFormat(szFormat, vl);
            va_end(vl);
        }
    }

    SString(const std::string& strText) : std::string(strText) {}

    SString& Format(const char* szFormat, ...)
    {
        va_list vl;

        va_start(vl, szFormat);
        SString& str = vFormat(szFormat, vl);
        va_end(vl);

        return str;
    }

    SString& vFormat(const char* szFormat, va_list vl);
    void     OnFormatException(const char* szFormat);
    void     OnInvalidParameter(const char* szFormat);

    // Access
    char& operator[](int iOffset) { return std::string::operator[](iOffset); }

    // Operators
    SString operator+(const char* other) const { return std::string(*this) + other; }
    SString operator+(const std::string& other) const { return std::string(*this) + other; }
    SString operator+(const SString& other) const { return std::string(*this) + other; }

    // Assignment
    operator const char* () const { return c_str(); }            // Auto assign to const char* without using c_str()
    const char* operator*() const { return c_str(); }

    // Functions
    void           Split(const SString& strDelim, std::vector<SString>& outResult, unsigned int uiMaxAmount = 0, unsigned int uiMinAmount = 0) const;
    bool           Split(const SString& strDelim, SString* pstrLeft, SString* pstrRight, int iIndex = 1) const;
    SString        SplitLeft(const SString& strDelim, SString* pstrRight = NULL, int iIndex = 1) const;
    SString        SplitRight(const SString& strDelim, SString* pstrLeft = NULL, int iIndex = 1) const;
    SString        Replace(const char* szOld, const char* szNew, bool bSearchJustReplaced = false) const;
    SString        ReplaceI(const char* szOld, const char* szNew, bool bSearchJustReplaced = false) const;
    SString        TrimStart(const char* szOld) const;
    SString        TrimEnd(const char* szOld) const;
    SString        ToLower() const;
    SString        ToUpper() const;
    SString        ConformLineEndings() const;
    bool           Contains(const SString& strOther) const;
    bool           ContainsI(const SString& strOther) const;
    bool           CompareI(const SString& strOther) const;
    bool           EqualsI(const SString& strOther) const { return CompareI(strOther); }
    SString        SubStr(int iPos, int iCount = 0x3fffffff) const;
    SString        Left(int iCount) const;
    SString        Right(int iCount) const;
    bool           EndsWith(const SString& strOther) const;
    bool           EndsWithI(const SString& strOther) const;
    bool           BeginsWith(const SString& strOther) const;
    bool           BeginsWithI(const SString& strOther) const;
    static SString Join(const SString& strDelim, const std::vector<SString>& parts, int iFirst = 0, int iCount = 0x3fffffff);
    void           AssignLeft(const char* szOther, uint uiMaxLength);
};
