/****************************************************************************
 * PseTask.h - Copyright 2017 Pu-Feng Du, Ph.D.                             *
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

#include "PseModule.h"
#include "PseResult.h"
#include "PseEngine.h"

#ifndef PSETASK_H_INCLUDED
#define PSETASK_H_INCLUDED

namespace UltraPse
{
    class PseTask
    {
        public:
            PseTask();
            ~PseTask();
            void SetInputFile(const char *fn);
            void SetOutputFile(const char *fn);
            void SetOutputFormat(const char *f);
            void SetTaskFile(const char *fn);
            void AddModule(const char *n);
            void AddProperty(const char *n);
            void ActivateQueryMode(const char *n);
            void AddExtraPara(const char *k, const char *v);
            void SetNotationName(const char *n);
            const char *GetNotationName();
            const char *GetInputFile();
            const char *GetOutputFile();
            const char *GetTaskFile();
            std::vector<const char*> *GetModNames();
            std::vector<const char*> *GetPropNames();
            std::map<const char *, const char *, StrKMC> *GetParas();
            PseResultFormat GetResultFormat();
            void SetValidate(bool v);
            bool GetValidate();
            void SetLuaExpr(const char *s) {LuaExpr = s;}
            const char *GetLuaExpr() {return LuaExpr;}
            const char *GetQueryMode() {return query;}
            bool IsQuery() {return QueryMode;}
            bool IsTask() {return HasTaskFile;}

            static PseTask *TaskVMPtr;

        private:
            bool IsUserModule(const char *s);
            void ValidateUserModule(const char *s);
            const char *InputFile;
            const char *OutputFile;
            const char *TaskFile;
            std::vector<const char *> *ModeList;
            std::vector<const char *> *PropList;
            std::map<const char *, const char *, StrKMC> *ExtraPara;
            PseResultFormat OutputFormat;
            bool QueryMode;
            bool AutoValidate;
            bool HasTaskFile;
            const char *query;
            const char *NotationName;
            const char *LuaExpr;
    };

};
#endif // PSETASK_H_INCLUDED
