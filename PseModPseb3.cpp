/****************************************************************************
 * PseModPseb3.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                       *
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
#include "LuaVM.h"
#include "LuaVMExt.h"
#include "PseModPseb3.h"
#include "PseEngine.h"
#include "PseCUI.h"
#include "BioNotations.h"

using namespace std;
using namespace UltraPse;

int UltraPse::GetPCList(lua_State *L)
{
    const char *n = lua_tostring(L, -1);
    PCProperty *p = UltraPse::PseEngine::EngineVMPtr->GetPropObject(n);
    lua_newtable(L);
    if (p)
    {
        lua_pushstring(L, "name");
        lua_pushstring(L, p->GetID());
        lua_settable(L, -3);
        lua_pushstring(L, "desc");
        lua_pushstring(L, p->GetComment());
        lua_settable(L, -3);
        BioNotations *b = p->GetNote();
        int cnt_notes = b->GetTotalNotations();
        int note_len = b->GetNotationLength();
        float *v = p->GetValues();
        char buf[note_len + 1]; // Dynamic allocation on stack, NOT heap!!
		memset(buf, 0, sizeof(char)*(note_len + 1)); //MUST manually init, for linux.
        for (int i = 0 ; i < cnt_notes ; i ++)
        {
            b->IndexToNotation(i, buf);
            char *t_note = strdup(buf);
            lua_pushstring(L, t_note);
            lua_pushnumber(L, v[i]);
            lua_settable(L, -3);
            free(t_note);
            t_note = 0;
        }
    }
    else
    {
        lua_pushstring(L, "name");
        lua_pushstring(L, "NA");
        lua_settable(L, -3);
        lua_pushstring(L, "desc");
        lua_pushstring(L, "Not found.");
        lua_settable(L, -3);
    }
    return 1;
}

int UltraPse::GetOptionList(lua_State *L)
{
    const char *k_l = UltraPse::PseEngine::EngineVMPtr->GetExtraPara(_cmd_lambda_name);
    const char *k_w = UltraPse::PseEngine::EngineVMPtr->GetExtraPara(_cmd_omega_name);
    int s_l = k_l ? atoi(k_l) : 10;
    float s_w = k_w ? atof(k_w) : 0.05;
    lua_newtable(L);
    lua_pushstring(L,"l");
    lua_pushnumber(L,s_l);
    lua_settable(L,-3);
    lua_pushstring(L,"w");
    lua_pushnumber(L,s_w);
    lua_settable(L,-3);
    return 1;
}

UltraPse::PseModule *UltraPse::PseModPseb3::Init(const char *n)
{
    return new PseModPseb3(n);
}

UltraPse::PseModPseb3::PseModPseb3(const char *n) : PseModuleRaw(n)
{
    Length = 0;
    UltraPse::LuaVM::LuaVMPtr->AddFunction("psebGetPCList", GetPCList);
    UltraPse::LuaVM::LuaVMPtr->AddFunction("psebGetOptions", GetOptionList);
}

UltraPse::PseModPseb3::~PseModPseb3()
{

}

int UltraPse::PseModPseb3::GetLength()
{
    return Length;
}

void UltraPse::PseModPseb3::Config()
{
    lua_State *L = UltraPse::LuaVM::LuaVMPtr->GetVMState();
    UltraPse::LuaVM::LuaVMPtr->VMSimpleCall(UltraPse::pseb3GetLengthProcName, 0, 1);
    if (lua_type(L, -1) != LUA_TNUMBER)
        ErrorAbort(ErrorCode::VMCoreAbort);
    Length = lua_tointeger(L, -1);
}

float *UltraPse::PseModPseb3::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    lua_State *L = UltraPse::LuaVM::LuaVMPtr->GetVMState();
    const char *seq = GetSeqClone();
    const char *id = GetSeqID();
    lua_getglobal(L, UltraPse::pseb3SeqProcName);
    lua_pushstring(L, id);
    lua_pushstring(L, seq);
    lua_call(L, 2, 2);
    int t_len = 0;
    if (lua_isnumber(L, -2))
    {
        t_len = lua_tonumber(L, -2);
    }
    if (t_len != Length)
        ErrorAbort(ErrorCode::VMCoreAbort);

    if (!lua_istable(L, -1))
        ErrorAbort(ErrorCode::VMCoreAbort);

    for (int i = 0 ; i < Length ; i ++)
    {
        lua_pushnumber(L, i + 1);
        lua_gettable(L, -2);
        r[i] = atof(lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    return r + Length;
}
