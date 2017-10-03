/****************************************************************************
 * LuaVM.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                           *
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

#include "LuaVM.h"
#include "Common.h"
#include "LuaVMExt.h"

using namespace UltraPse;

LuaVM* UltraPse::LuaVM::LuaVMPtr = 0;

UltraPse::LuaVM::LuaVM()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    s = LuaVMStatus::Created;
    LuaVMPtr = this;
}

UltraPse::LuaVM::~LuaVM()
{
    lua_close(L);
}

bool UltraPse::LuaVM::AddFunction(const char *FuncName, lua_CFunction FPtr)
{
    if (!(lua_getglobal(L, FuncName)))
    {
        lua_register((L),(FuncName),(FPtr));
        return true;
    }
    return false;
}

void UltraPse::LuaVM::AddInternalFunctions()
{
    for (auto &tf : VMExtLib)
    {
        if (!AddFunction(tf.first, tf.second))
        {
            ErrorAbort(ErrorCode::VMCoreAbort);
        }
    }
}

void UltraPse::LuaVM::InitVM()
{
    if (s != LuaVMStatus::Created)
        return;
    AddInternalFunctions();
    s = LuaVMStatus::Initialized;
}

void UltraPse::LuaVM::ConfigVM()
{
    if (s != LuaVMStatus::Initialized)
        return;
    VMCall(UltraPse::LuaVMConfigProcName);
    s = LuaVMStatus::Configured;
}

void UltraPse::LuaVM::VMErrorAbort(int e)
{
    if (e)
    {
        char *tMsg = strdup(lua_tostring(L, -1));
        lua_close(L);
        L = 0;
        ErrorAbort(ErrorCode::VMCoreAbort);
    }
}

void UltraPse::LuaVM::VMCall(const char *proc_name)
{
    lua_getglobal(L, proc_name);
    VMErrorAbort(lua_pcall(L,0,0,0));
}

void UltraPse::LuaVM::VMSimpleCall(const char *proc_name, int nargs, int nres)
{
    if (lua_getglobal(L, proc_name) != LUA_TNIL)
        lua_call(L, nargs, nres);
    else
        ErrorAbort(ErrorCode::NoProcInTaskFile);
}

lua_State *UltraPse::LuaVM::GetVMState()
{
    return L;
}

void UltraPse::LuaVM::Execute(const char *fn)
{
    TaskFile = fn;
    VMErrorAbort(luaL_loadfile(L, fn) || lua_pcall(L,0,0,0));
    s = LuaVMStatus::TaskLoaded;
}

void UltraPse::LuaVM::ExecChunk(const char *prog)
{
    if (prog)
        VMErrorAbort(luaL_loadstring(L, prog) || lua_pcall(L,0,0,0));
}

void UltraPse::LuaVM::VMSendSerial(int *v, int n)
{
    lua_newtable(L);
    const char *Length = "Length";
    lua_pushstring(L, Length);
    lua_pushnumber(L, n);
    lua_settable(L, -3);

    for (int i = 0; i < n ; i ++)
    {
        lua_pushnumber(L, i + 1);
        lua_pushnumber(L, v[i]);
        lua_settable(L, -3);
    }
}
