#include <lua.hpp>
#include "../../PseModule.h"

#ifndef PSEMODINTERFACE_H_INCLUDED
#define PSEMODINTERFACE_H_INCLUDED

namespace UltraPse
{
    extern PseModuleInterface ModuleInterfaces[];
    extern VMExtInterface VMInterfaces[];
};

extern "C"
{
    UltraPse::PseModuleInterface *GetInterfaces();
    UltraPse::VMExtInterface *GetVMInterfaces();
};

#endif // PSEMODINTERFACE_H_INCLUDED
