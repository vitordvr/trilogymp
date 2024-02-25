#include "Main.h"
#include "Utils.h"

static SString gs_path;
static SString gs_gtaPath;
static HANDLE  gs_hMutex = NULL;

namespace fs = std::filesystem;

ePathResult DiscoverPath()
{
    SString tempGamePath = "D:\\Games\\GTA\\Gameface\\Binaries\\Win64";

    if (!FileExists(PathJoin(tempGamePath, GTA_EXE_NAME)))
    {
        return PATH_MISSING;
    }

    gs_gtaPath = tempGamePath;
    return PATH_OK;
}


SString GetGamePath()
{
    DiscoverPath();
    return gs_gtaPath;
}


auto GetGameExecutable() -> std::filesystem::path
{
    static const auto directory = fs::path{ FromUTF8(GetGamePath()) };
    return directory / GTA_EXE_NAME;
};
