/****************************************************************************
 * Operator.h - Copyright 2017 Pu-Feng Du, Ph.D.                            *
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

#include "PseEngine.h"
#include "PseTask.h"
#include "BioProcessor.h"
#include "BlockFile.h"
#include "PCLib.h"
#include "LuaVM.h"

#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED

namespace UltraPse
{
    class Operator
    {
        public:
            Operator();
            ~Operator();
            void SetTask(PseTask *t);
            void BatchNotify(); //This will be called after a batch of sequences parsed from files
            void CreatNotation();
            void CreatDataParsers();
            void CreatEngine();
            void CreateLuaVM();
            void PrepareFileHandles();
            void Prepare();
            void Execute();
            void Query(const char *q);
        private:
            void ExportResults();
            void ClearResultBuffer();

            int count_batch;
            PseEngine *Engine;
            BioSequenceList *DataSource;
            BioNotations *Note;
            BlockFile *FileTrunk;
            FastaParser *Parser;
            PCLibrary *PCLib;
            LuaVM *lua_vm;
            PseTask *Task;
            FILE *ifHandle;
            FILE *ofHandle;
            std::vector<PseResult *> *Results;
            std::vector<PCProperty *> *ActiveProps;

    };
};



#endif // OPERATOR_H_INCLUDED
