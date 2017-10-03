/****************************************************************************
 * PseModLua.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                         *
 *                                                                          *
 * This file is a part of the software: UltraPse                            *
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

#include <lua.hpp>
#include "PseModLua.h"
#include "LuaVM.h"
#include "LuaVMExt.h"

using namespace std;
using namespace UltraPse;

PseModule *UltraPse::PseModLua::Init(const char *n)
{
    return new PseModLua(n);
}

UltraPse::PseModLua::PseModLua(const char *n) : PseModule (n)
{
    Length = 0;
    if (UltraPse::LuaVM::LuaVMPtr->GetStatus() != LuaVMStatus::TaskLoaded)
    {
        ErrorAbort(ErrorCode::NoTaskFile);
    }
}

UltraPse::PseModLua::~PseModLua()
{

}

float *UltraPse::PseModLua::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    lua_State *L = UltraPse::LuaVM::LuaVMPtr->GetVMState();
    if (lua_getglobal(L, UltraPse::LuaVMProcComputeStub) != LUA_TNIL)
    {
        UltraPse::LuaVM::LuaVMPtr->VMSendSerial(IndexSerial, LengthOfIndex);
        lua_call(L, 1, 1);
        if (lua_getfield(L, -1, "Length") != LUA_TNUMBER)
        {
            ErrorAbort(ErrorCode::VMCoreAbort);
        }
        int n = lua_tointeger(L, -1);
        lua_pop(L, 1);
        for (int i = 0; i < n ; i ++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L, -2);
            r[i] = atof(lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    else
        ErrorAbort(ErrorCode::NoProcInTaskFile);
    return r + Length;
}

void UltraPse::PseModLua::Config()
{
    lua_State *L = UltraPse::LuaVM::LuaVMPtr->GetVMState();
    if (lua_getglobal(L, UltraPse::LuaVMProcConfigStub) != LUA_TNIL)
    {
        lua_call(L, 0, 1);
        if (lua_type(L, -1) == LUA_TNUMBER)
        {
            int l = lua_tointeger(L, -1);
            Length = l;
        }
        else
            ErrorAbort(ErrorCode::NoProcInTaskFile);
    }
    else
        ErrorAbort(ErrorCode::NoProcInTaskFile);
}
