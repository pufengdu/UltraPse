/****************************************************************************
 * BlockFile.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                         *
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

#include "BlockFile.h"
#include "FastaParser.h"
#include "Config.h"
#include "Common.h"

using namespace UltraPse;
using namespace std;

const int UltraPse::MaxBufferSize = _CONF_MAX_BUF_SIZE_; //Max buffer size, configurable as a MACRO

UltraPse::BlockFile::BlockFile(FILE *ifh, FastaParser *p)
{
    fh = ifh;
    if (fh != stdin)
    {
        FileLength = filelength(fileno(fh));
        if (FileLength <= MaxBufferSize)
        {
            BatchMode = false;
            BufferSize = FileLength;
        }
        else
        {
            BatchMode = true;
            BufferSize = MaxBufferSize;
        }
    }
    else
    {
        BatchMode = true;
        BufferSize = MaxBufferSize;
    }

    b = new Byte[BufferSize];
    memset(b, 0, sizeof(Byte) * BufferSize);
    CurrentLine_Offset = 0;
    Critical_Offset = 0;
    Buffer_Offset = 0;
    EffectSize = 0;
    Assoc_Parser = p;
    //FileName = fn;
}

UltraPse::BlockFile::~BlockFile()
{
    //fclose(fh);
    //fh = 0;
    delete [] b;
    b = 0;
}

void UltraPse::BlockFile::Execute()
{
    do
    {
        FillBuffer();
        ProcBuffer();
        memset(b, 0, sizeof(Byte) * BufferSize);
    } while (Assoc_Parser->GetSignal() != FastaParserSignal::Release);
}

void UltraPse::BlockFile::FillBuffer()
{
    fseek(fh,Critical_Offset,SEEK_SET);
    EffectSize = fread(b,sizeof(Byte),BufferSize, fh);
    Buffer_Offset = Critical_Offset;
}

void UltraPse::BlockFile::ProcBuffer()
{
    char *Current_Line = (char*)b;
    for (int i = 0 ; i < EffectSize ; i ++)
    {
        if (b[i] == '\n')
        {
            CurrentLine_Offset = Current_Line - (char*)b;
            b[i] = 0;
            Current_Line = Trim(Current_Line);
            Assoc_Parser->Transfer(Current_Line);
            FastaParserSignal t_sig = Assoc_Parser->GetSignal();
            if (t_sig == FastaParserSignal::Refresh)
            {
                Critical_Offset = Buffer_Offset + CurrentLine_Offset;
            }
            Current_Line = (char*)(b + i + 1);
        }
    }
    if (!BatchMode)
    {
        Assoc_Parser->Finish();
    }
    else
    {
        if (EffectSize < BufferSize)
        {
            Assoc_Parser->Finish();
        }
        else
        {
            Assoc_Parser->Reset();
        }
    }
}







