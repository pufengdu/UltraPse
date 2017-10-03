/****************************************************************************
 * LuaVMExt.h - Copyright 2017 Pu-Feng Du, Ph.D.                            *
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

#ifndef LUAVMEXT_H_INCLUDED
#define LUAVMEXT_H_INCLUDED

namespace UltraPse
{
    extern const char *LuaVMConfigProcName;
    extern const char *LuaVMProcConfigStub;
    extern const char *LuaVMProcComputeStub;

    extern const char *pseb3SeqProcName;
    extern const char *pseb3GetLengthProcName;

    char *GetLuaTableFieldAsString(lua_State *L, int table_index, const char *FieldName);
    int GetLuaTableFieldAsInteger(lua_State *L, int table_index, const char *FieldName);
    int LoadPseModule(lua_State *L);
    int LoadUserNotation(lua_State *L);
    int ListNotations(lua_State *L);
    int SetInputFile(lua_State *L);
    int GetInputFile(lua_State *L);
    int SetOutputFile(lua_State *L);
    int GetOutputFile(lua_State *L);
    int GetOption(lua_State *L);
    int GetNoteNames(lua_State *L);
    int GetNotation(lua_State *L);
    int SetNotation(lua_State *L);
    int AddModule(lua_State *L);
    int GetAllModuleNames(lua_State *L);
    int GetActiveModuleNames(lua_State *L);
    int LoadUserProperty(lua_State *L);
    int AddProperty(lua_State *L);
    int GetActivePropNames(lua_State *L);
    int GetPropLibNames(lua_State *L);

    void LoadUserBinaryModule(const char *fn, bool LoadExtFuncInRealTime);
};

#endif // LUAVMEXT_H_INCLUDED
