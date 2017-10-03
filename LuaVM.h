/****************************************************************************
 * LuaVM.h - Copyright 2017 Pu-Feng Du, Ph.D.                               *
 *                                                                          *
 * This file is a part of the software: UltraPse.                           *
 * UltraPse is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * UltraPse is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with UltraPse.  If not, see <http://www.gnu.org/licenses/>.        *
 ****************************************************************************/


#include "Common.h"

#ifndef _LUAVM_H_
#define _LUAVM_H_
/**
  * LUAVM.H defines the virtual machine of LUA interpreter. LUAVM allows data exchange and interoperability between C/C++ code
  * and LUA codes.
  * @author Pufeng Du.
*/

// The following will require adding INCLUDE path and LIB path in compiling process
#ifdef __cplusplus
#include <lua.hpp>
#else
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

#include <map>
using namespace std;
//luavm functions
//allow lua to modify a number of C variables
//allow lua to call a number of C functions
//Evaluate chunks of lua codes for init and config

//The LuaVM is the core of UltraPse, whenever LuaVM encounters errors, the whole program will abort.
//typedef int (*LuaExtFunction)(lua_State *);

namespace UltraPse
{
    enum LuaVMStatus {
        Created,
        Initialized,
        Configured,
        TaskLoaded,
        Destroyed
    };

    class LuaVM
    {
        public:
            LuaVM();
            ~LuaVM();
            void InitVM();
            void ConfigVM();
            void Execute(const char *fn);
            void VMCall(const char *proc_name);
            void VMSimpleCall(const char *proc_name, int nargs, int nres);
            void ExecChunk(const char *prog);
            lua_State *GetVMState();
            void AddInternalFunctions();
            bool AddFunction(const char *FuncName, lua_CFunction FPtr);
            LuaVMStatus GetStatus() {return s;}
            void VMSendSerial(int *v, int n);

            static LuaVM *LuaVMPtr;
        private:
            lua_State *L;
            LuaVMStatus s;
            void VMErrorAbort(int e);
            const char *TaskFile;
    };

    extern std::map<const char *, lua_CFunction, StrKMC> VMExtLib;
};

#endif // _LUAVM_H_
