#include "Singleton.h"
#include "ModuleLoader.h"

class Core;

#pragma once

extern class Core* gs_Core;


class Core : public Singleton<Core>
{
public:
    Core();
    ~Core();


    // Monolits
    void CreateGUI();
    void DestroyGUI();


    void ApplyHooks();

    bool AreModulesLoaded();


    void SetModulesLoaded(bool state);
private:
    ModuleLoader m_pGUIModule;
    bool m_pModulesLoaded;
};
