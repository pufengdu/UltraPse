/****************************************************************************
 * LuaVMExt.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                           *
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

#include <dlfcn.h>
#include "LuaVM.h"
#include "LuaVMExt.h"
#include "BioNotations.h"
#include "PseTask.h"
#include "Common.h"
#include "PseCUI.h"
#include "RawProperty.h"
#include "PCLib.h"

using namespace std;
using namespace UltraPse;

const char *UltraPse::LuaVMConfigProcName = "UltraPseVMConfig";
const char *UltraPse::LuaVMProcConfigStub = "UPseConfig";
const char *UltraPse::LuaVMProcComputeStub = "UPseCompute";
const char *UltraPse::pseb3SeqProcName = "pseb3_seq_proc";
const char *UltraPse::pseb3GetLengthProcName = "pseb3_length";

map<const char *, lua_CFunction, StrKMC> UltraPse::VMExtLib
(
    {
        {"LoadModule", UltraPse::LoadPseModule},
        {"AddModule", UltraPse::AddModule},
        {"GetAllModules", UltraPse::GetAllModuleNames},
        {"GetActiveModules", UltraPse::GetActiveModuleNames},
        {"DefineNotation", UltraPse::LoadUserNotation},
        {"PrintNotes", UltraPse::ListNotations},
        {"GetNotation", UltraPse::GetNotation},
        {"SetNotation",UltraPse::SetNotation},
        {"GetNoteNames", UltraPse::GetNoteNames},
        {"SetInputFile", UltraPse::SetInputFile},
        {"GetInputFile", UltraPse::GetInputFile},
        {"SetOutputFile", UltraPse::SetOutputFile},
        {"GetOutputFile", UltraPse::GetOutputFile},
        {"GetOption", UltraPse::GetOption},
        //{"psebGetOptions", UltraPse::GetOption},
        {"DefineProperty", UltraPse::LoadUserProperty},
        {"AddProperty", UltraPse::AddProperty},
        {"GetActiveProperties", UltraPse::GetActivePropNames},
        {"GetPropertyLibNames", UltraPse::GetPropLibNames}
    },
    UltraPse::StrKeyMapComp
);

char *UltraPse::GetLuaTableFieldAsString(lua_State *L, int table_index, const char *FieldName)
{
    if (lua_getfield(L, table_index, FieldName) == LUA_TSTRING)
    {
        return strdup(lua_tostring(L, -1));
    }
    else
    {
        return 0;
    }
}

int UltraPse::GetLuaTableFieldAsInteger(lua_State *L, int table_index, const char *FieldName)
{
    if (lua_getfield(L, table_index, FieldName) == LUA_TNUMBER)
    {
        return lua_tointeger(L, -1);
    }
    else
    {
        ErrorAbort(ErrorCode::VMCoreAbort);
    }
    return 0;
}

int UltraPse::AddProperty(lua_State *L)
{
    const char *p = lua_tostring(L, -1);
    UltraPse::PseTask::TaskVMPtr->AddProperty(strdup(p));
    return 0;
}

int UltraPse::LoadUserNotation(lua_State *L)
{
    if (lua_type(L,1) == LUA_TTABLE)
    {
        char *NotationName = GetLuaTableFieldAsString(L, 1, "Name");
        if (!NotationName)
            ErrorAbort(ErrorCode::VMCoreAbort);
        char *BaseNotes = GetLuaTableFieldAsString(L, 1, "Base");
        if (!BaseNotes)
            ErrorAbort(ErrorCode::VMCoreAbort);
        char *ReduceRules = GetLuaTableFieldAsString(L, 1, "ReduceMap");
        int NoteLen = GetLuaTableFieldAsInteger(L,1,"Length");
        /*BioNoteDef *t = new BioNoteDef();
        t->BaseNotes = BaseNotes;
        t->Name = NotationName;
        t->Len = NoteLen;
        t->ReduceRule = ReduceRules;
        t->UseMap = true;*/
        UltraPse::BioNoteDefLib[strupr(NotationName)] = {strupr(NotationName), BaseNotes, NoteLen, true, ReduceRules};

    }
    else
    {
        ErrorAbort(ErrorCode::VMCoreAbort);
    }
    return 0;
}

int UltraPse::LoadUserProperty(lua_State *L)
{
    if (lua_type(L,1) == LUA_TTABLE)
    {
        RawProperty *r = 0;
        lua_getfield(L, 1, "Template");
        const char *tempname = lua_tostring(L, -1);
        lua_getfield(L, 1, "ID");
        const char *tid = lua_tostring(L,-1);
        lua_getfield(L,1,"Comment");
        const char *tcom = lua_tostring(L,-1);
        if (tempname && tid && tcom)
        {
            r = new RawProperty();
            r->SetID(tid);
            r->SetTemplate(tempname);
            r->SetComment(tcom);
        }
        lua_getfield(L, 1, "Values");
        int topidx = lua_gettop(L);
        lua_pushnil(L);
        while (lua_next(L, topidx) != 0)
        {
            const char *k = lua_tostring(L, -2);
            const char *v = lua_tostring(L, -1);
            r->AddValue(k, (float)atof(v));
            lua_pop(L,1);
        }
        if (RawPropLib.find(tid) == RawPropLib.end())
        {
            RawPropLib[tid] = r;
        }
    }
    return 0;
}

int UltraPse::LoadPseModule(lua_State *L)
{
    const char *t = strdup(lua_tostring(L, -1));
    LoadUserBinaryModule(t, true);
    _MemoryRepo.push_back((void*)t);
    return 0;
}

int UltraPse::AddModule(lua_State *L)
{
    const char *mod_name = lua_tostring(L, -1);
    if(EngineModuleLibrary.find(mod_name) != EngineModuleLibrary.end())
    {
        UltraPse::PseTask::TaskVMPtr->AddModule(mod_name);
        lua_pushnumber(L,1);
        return 1;
    }
    else
    {
        lua_pushnumber(L,0);
        return 1;
    }
}

int UltraPse::GetActivePropNames(lua_State *L)
{
    lua_newtable(L);
    int i = 1;
    vector<const char *> *v = UltraPse::PseTask::TaskVMPtr->GetPropNames();
    for (const char *p : *v)
    {
        lua_pushnumber(L, i);
        lua_pushstring(L, p);
        lua_settable(L, -3);
        i ++;
    }
    return 1;
}

int UltraPse::GetPropLibNames(lua_State *L)
{
    lua_newtable(L);
    int i = 1;
    for (auto &p : PropertyLibRegistry)
    {
        lua_pushnumber(L, i);
        lua_pushstring(L, (const char *)p.first);
        lua_settable(L, -3);
        i ++;
    }
    return 1;
}

int UltraPse::GetAllModuleNames(lua_State *L)
{
    lua_newtable(L);
    int i = 1;
    for (auto &p : EngineModuleLibrary)
    {
        lua_pushnumber(L,i);
        lua_pushstring(L, (const char *)p.first);
        lua_settable(L,-3);
        i++;
    }
    return 1;
}

int UltraPse::GetActiveModuleNames(lua_State *L)
{
    lua_newtable(L);
    int i = 1;
    vector<const char *> *modes = UltraPse::PseTask::TaskVMPtr->GetModNames();
    for (auto &p : *modes)
    {
        lua_pushnumber(L, i);
        lua_pushstring(L, p);
        lua_settable(L, -3);
        i ++;
    }
    return 1;
}

int UltraPse::ListNotations(lua_State *L)
{
    BioNotations::DisplayLibrary();
    return 0;
}

int UltraPse::SetInputFile(lua_State *L)
{
    const char *fn = lua_tostring(L, -1);
    UltraPse::PseTask::TaskVMPtr->SetInputFile(fn);
    return 0;
}

int UltraPse::GetInputFile(lua_State *L)
{
    const char *fn = UltraPse::PseTask::TaskVMPtr->GetInputFile();
    if (fn)
    {
        lua_pushstring(L, fn);
        return 1;
    }
    else
    {
        lua_pushstring(L, "stdin");
        return 1;
    }
}

int UltraPse::SetOutputFile(lua_State *L)
{
    const char *fn = lua_tostring(L, -1);
    UltraPse::PseTask::TaskVMPtr->SetOutputFile(fn);
    return 0;
}

int UltraPse::GetOutputFile(lua_State *L)
{
    const char *fn = UltraPse::PseTask::TaskVMPtr->GetOutputFile();
    if (fn)
    {
        lua_pushstring(L, fn);
        return 1;
    }
    else
    {
        lua_pushstring(L, "stdout");
        return 1;
    }
}

int UltraPse::GetOption(lua_State *L)
{
    const char *opt_name = lua_tostring(L,-1);
    map<const char *, const char *, StrKMC> *t = UltraPse::PseTask::TaskVMPtr->GetParas();
    if (strcasecmp(opt_name, _cmd_lambda_name) == 0)
    {
        lua_pushstring(L, (*t)[_cmd_lambda_name]);
        return 1;
    }
    else if (strcasecmp(opt_name, _cmd_omega_name) == 0)
    {
        lua_pushstring(L, (*t)[_cmd_omega_name]);
        return 1;
    }
    else if (strcasecmp(opt_name, _cmd_subtype) == 0)
    {
        lua_pushstring(L, (*t)[_cmd_subtype]);
        return 1;
    }
    else if (strcasecmp(opt_name, _cmd_ktuple) == 0)
    {
        lua_pushstring(L, (*t)[_cmd_ktuple]);
        return 1;
    }
    else
    {
        lua_pushstring(L, "");
        return 1;
    }
}

int UltraPse::SetNotation(lua_State *L)
{
    const char *note = lua_tostring(L,-1);
    if (BioNoteDefLib.find(note) != BioNoteDefLib.end())
    {
        UltraPse::PseTask::TaskVMPtr->SetNotationName(note);
    }
    else
    {
        ErrorAbort(ErrorCode::VMCoreAbort);
    }
    return 0;
}

int UltraPse::GetNotation(lua_State *L)
{
    const char *note = UltraPse::PseTask::TaskVMPtr->GetNotationName();
    if (note)
    {
        lua_pushstring(L, note);
        return 1;
    }
    else
    {
        lua_pushstring(L,"");
        return 1;
    }
}

int UltraPse::GetNoteNames(lua_State *L)
{
    lua_newtable(L);
    int i = 1;
    for (auto &p : BioNoteDefLib)
    {
        lua_pushnumber(L, i);
        lua_pushstring(L, (const char*)p.first);
        lua_settable(L,-3);
        i++;
    }
    return 1;
}

void UltraPse::LoadUserBinaryModule(const char *fn, bool LoadExtFuncInRealTime)
{
    if (_HandleRepo.find(fn) != _HandleRepo.end())
    {
        return;
    }

    void *ModuleHandle = dlopen(fn, RTLD_LAZY);
    if (!ModuleHandle)
        ErrorAbort(ErrorCode::SharedObjectError);

    _HandleRepo[fn] = ModuleHandle;

    ModIFunc PseIF = (ModIFunc)dlsym(ModuleHandle, "GetInterfaces");
    VMIFunc VMIF = (VMIFunc)dlsym(ModuleHandle,"GetVMInterfaces");
    if (!PseIF || !VMIF)
        ErrorAbort(ErrorCode::SharedObjectError);

	PseModuleInterface *UserModIF = PseIF();
    VMExtInterface *UserVMIF = VMIF();
    while (UserModIF->ModuleName)
    {
        PseEngine::RegisterModuleToLibrary(UserModIF->ModuleName,UserModIF->InitFunc);
        UserModIF ++;
    }
    while (UserVMIF->VMFunction)
    {
        if (VMExtLib.find(UserVMIF->VMFunction) == VMExtLib.end())
        {
            VMExtLib[UserVMIF->VMFunction] = UserVMIF->CFPtr;
            if (LoadExtFuncInRealTime && UltraPse::LuaVM::LuaVMPtr)
            {
                LuaVM::LuaVMPtr->AddFunction(UserVMIF->VMFunction, UserVMIF->CFPtr);
            }
        }
        UserVMIF ++;
    }
}
