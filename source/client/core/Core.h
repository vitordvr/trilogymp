#include "Singleton.h"

class Core;

#pragma once

extern class Core* gs_Core;


class Core : public Singleton<Core>
{
public:
    Core();
    ~Core();
};
