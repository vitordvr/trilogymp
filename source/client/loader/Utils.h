#pragma once
#include <iostream>
#include <filesystem>

enum ePathResult {
    PATH_UNICODE_CHARS,
    PATH_MISSING,
    PATH_OK
};



ePathResult     DiscoverPath();
SString         GetGamePath();
auto            GetGameExecutable() -> std::filesystem::path;
bool CreateSingleInstanceMutex();
