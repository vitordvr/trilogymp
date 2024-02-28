#include "StdInc.h"
#include "Core.h"
#include <app/Debug.h>
#include <renderer/Renderer.hpp>

#include <Windows.h>
#include <delayimp.h>
#pragma comment(lib, "delayimp")



extern Core* gs_Core;


template <>
Core* Singleton<Core>::m_pSingleton = NULL;




Core::Core()
{
    gs_Core = this;
};


Core::~Core()
{
    delete gs_Core;
}
