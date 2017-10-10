#include "../../PseModule.h"
#include "../Template/PseModInterface.h"
#include <lua.hpp>
#ifndef PSEMODDUMMY_H_INCLUDED
#define PSEMODDUMMY_H_INCLUDED

//Module Extended Lua VM Functions
extern "C"
{
    int DummyLuaExtFunction1(lua_State *L);
    int DummyLuaExtFunction2(lua_State *L);
}

namespace UltraPse
{
    //User defined engine module
    class PseModDummy : public PseModule
    {
        public:
            PseModDummy(const char *n);
            ~PseModDummy();
            void Config();
            int GetLength();
            float *Compute(int *IndexSerial, int LengthOfIndex, float *r);
            static PseModule *Init(const char *n);
        private:
            int Length;
    };
};

#endif // PSEMODDUMMY_H_INCLUDED
