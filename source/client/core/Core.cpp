#include "StdInc.h"
#include "Core.h"

extern Core* gs_Core;


template <>
Core* Singleton<Core>::m_pSingleton = NULL;


Core::Core()
{
    gs_Core = this;

    std::setlocale(LC_ALL, "C");
    std::setlocale(LC_CTYPE, "");


    std::cout << "Core has injected" << std::endl;
};


Core::~Core()
{
}
