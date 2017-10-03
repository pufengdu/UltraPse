/****************************************************************************
 * Common.h - Copyright 2017 Pu-Feng Du, Ph.D.                              *
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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <sys/stat.h>
#include <dlfcn.h>

#include "Config.h"

// Common definitions
typedef unsigned char Byte;

#ifdef __COMP_FLAG_FLOAT__
typedef float decimal;
#else
typedef double decimal;
#endif // __COMP_FLAG_FLOAT__

namespace UltraPse
{
    typedef bool (*StrKMC) (const char *, const char *);
    enum ErrorCode {
        VMCoreAbort,
        NormalTermination,
        InvalideNotations,
        InvalideReducingRule,
        MultiByteMapNotCreated,
        NoSuchNotationType,
        PCLibInitError,
        SharedObjectError,
        InputFileError,
        OutputFileExisted,
        OutputFileCreateError,
        UndefinedOutputFormat,
        CannotOpenTaskFile,
        SequenceWithUndefinedNotations,
        UndefinedEngineModule,
        InvalideArgument,
        NoSuchProperty,
        NoTaskFile,
        NoProcInTaskFile
    };
    void ErrorAbort(ErrorCode e);
    off_t FileSize(const char *fn);
	int filelength(int fd);
	char *strupr(char *s);
    char *Trim(char *s);
    char **LoadTextLines(const char *fn);
    bool CheckPrefix(char *s, const char *prefix);
    template <class T>
    int qSortComp(const void *a, const void *b)
    {
        return *(T*)a - *(T*)b;
    }

    bool StrKeyMapComp(const char *a, const char *b);
    extern char FastaCommentSep;
    float *NormalizeValues(float *data, int n);
    int SplitCSV(char *s, char ***out_fields);
    char *BlockToCode(Byte *b, char *CodePrefix);

    extern std::vector<void *> _MemoryRepo;
    extern std::map<const char *, void *, StrKMC> _HandleRepo;
};

#endif // _COMMON_H_
