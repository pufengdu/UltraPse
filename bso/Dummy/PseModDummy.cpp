#include "PseModDummy.h"
#include "../Template/PseModInterface.h"

using namespace std;
using namespace UltraPse;

//Module Registry
PseModuleInterface UltraPse::ModuleInterfaces[] =
{
    {"DUMMY", UltraPse::PseModDummy::Init},
    {0,0}
};

VMExtInterface UltraPse::VMInterfaces[] =
{
    {"DummyProc1", DummyLuaExtFunction1},
    {"DummyProc2", DummyLuaExtFunction2},
    {0,0}
};

//Lua VM function implementation
int DummyLuaExtFunction1(lua_State *L)
{
    printf("Running Ext Function 1\n");
    return 0;
}
int DummyLuaExtFunction2(lua_State *L)
{
    printf("Running Ext Function 2\n");
    return 0;
}

//Engine module implementation
UltraPse::PseModDummy::PseModDummy(const char *n) : PseModule(n)
{
    Length = 1;
}

UltraPse::PseModDummy::~PseModDummy()
{

}

void UltraPse::PseModDummy::Config()
{

}

int UltraPse::PseModDummy::GetLength()
{
    return Length;
}

float *UltraPse::PseModDummy::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    r[0] = static_cast<float>(LengthOfIndex);
    return r + 1;
}

PseModule *UltraPse::PseModDummy::Init(const char *n)
{
    return new PseModDummy(n);
}
