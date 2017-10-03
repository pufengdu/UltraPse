/****************************************************************************
 * BlockFile.h - Copyright 2017 Pu-Feng Du, Ph.D.                           *
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

#include "FastaParser.h"

#ifndef BLOCKFILE_H
#define BLOCKFILE_H

namespace UltraPse
{
    extern const int MaxBufferSize;

    class BlockFile
    {
        public:
            BlockFile(FILE *ifh, FastaParser *p);
            ~BlockFile();
            void Execute();

        private:
            void ProcBuffer();
            void FillBuffer();
            bool BatchMode;
            int BufferSize;
            int EffectSize;
            Byte *b; //Reading Buffer
            FastaParser *Assoc_Parser;
            int Critical_Offset;
            int Buffer_Offset;
            int FileLength;
            int CurrentLine_Offset;
            const char *FileName;
            FILE *fh;
    };
};

#endif // BLOCKFILE_H
