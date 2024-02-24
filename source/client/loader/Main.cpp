#include "StdInc.h"


#include <windows.h>
#include <iostream>


TRILOGY_API int Run(HINSTANCE hLauncherInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LaunchGame();

    return 1;
}
