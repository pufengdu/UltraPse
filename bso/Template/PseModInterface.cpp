#include "PseModInterface.h"

using namespace UltraPse;

PseModuleInterface *GetInterfaces()
{
    return UltraPse::ModuleInterfaces;
}

VMExtInterface *GetVMInterfaces()
{
    return UltraPse::VMInterfaces;
}
