/****************************************************************************
 * PseTask.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                           *
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

#include "PseTask.h"
#include "LuaVMExt.h"
#include <unistd.h>
using namespace UltraPse;
using namespace std;

PseTask* PseTask::TaskVMPtr = 0;

UltraPse::PseTask::PseTask()
{
    InputFile = 0;
    OutputFile = 0;
    TaskFile = 0;
    OutputFormat = UltraPse::PseResultFormat::CSV;
    QueryMode = false;
    AutoValidate = false;
    ModeList = new vector<const char *>();
    PropList = new vector<const char *>();
    ExtraPara = new map<const char*, const char *, StrKMC>(StrKeyMapComp);
    NotationName = 0;
    TaskVMPtr = this;
    HasTaskFile = false;
    LuaExpr = 0;
}

UltraPse::PseTask::~PseTask()
{
    delete ModeList;
    delete PropList;
    delete ExtraPara;
}

const char *UltraPse::PseTask::GetTaskFile()
{
    return TaskFile;
}

void UltraPse::PseTask::AddExtraPara(const char *k, const char *v)
{
    (*ExtraPara)[k] = v;
}

void UltraPse::PseTask::SetTaskFile(const char *fn)
{
    if (access(fn, R_OK) == 0)
    {
        TaskFile = fn;
        HasTaskFile = true;
        return;
    }
    ErrorAbort(ErrorCode::CannotOpenTaskFile);
}
void UltraPse::PseTask::ActivateQueryMode(const char *n)
{
    QueryMode = true;
    query = n;
}

void UltraPse::PseTask::AddModule(const char *n)
{
    if (IsUserModule(n))
    {
        ValidateUserModule(n);
    }
    else
    {
        ModeList->push_back(n);
    }
}

bool UltraPse::PseTask::IsUserModule(const char *s)
{
    if (strncasecmp(s,"USER",4) == 0)
        return true;
    else
        return false;
}

void UltraPse::PseTask::ValidateUserModule(const char *s)
{
    char **f = 0;
    int cf = SplitCSV(strdup(s), &f);
    if (cf != 4)
    {
        delete [] f;
        ErrorAbort(ErrorCode::InvalideArgument);
        return;
    }
    char *SOName = f[1];
    char *ModName = f[2];
    if (access(SOName, 0) != 0)
    {
        ErrorAbort(ErrorCode::InvalideArgument);
        return;
    }
    LoadUserBinaryModule(SOName, false);
    ModeList->push_back(ModName);
    delete [] f;
}

void UltraPse::PseTask::AddProperty(const char *n)
{
    PropList->push_back(n);
}

void UltraPse::PseTask::SetInputFile(const char *fn)
{
    if (!fn)
    {
        ErrorAbort(ErrorCode::InputFileError);
    }
    if (strlen(fn) < 1)
    {
        ErrorAbort(ErrorCode::InputFileError);
    }
    if (strncasecmp(fn,"-",1) == 0)
    {
        InputFile = 0;
        return;
    }
    InputFile = fn;
    if (access(InputFile, R_OK))
    {
        InputFile = 0;
        UltraPse::ErrorAbort(ErrorCode::InputFileError);
    }
}

const char *UltraPse::PseTask::GetInputFile()
{
    return this->InputFile;
}

void UltraPse::PseTask::SetOutputFile(const char *fn)
{
    if (!fn)
    {
        ErrorAbort(ErrorCode::OutputFileCreateError);
    }
    if (strlen(fn) < 1)
    {
        ErrorAbort(ErrorCode::OutputFileCreateError);
    }
    if (strncasecmp(fn,"-",1) == 0)
    {
        OutputFile = 0;
        return;
    }
    OutputFile = fn;
    if (!access(OutputFile, R_OK))
    {
        OutputFile = 0;
        UltraPse::ErrorAbort(ErrorCode::OutputFileExisted);
    }
}

const char *UltraPse::PseTask::GetOutputFile()
{
    return OutputFile;
}

void UltraPse::PseTask::SetOutputFormat(const char *OutputFormatString)
{
    if (strncasecmp(OutputFormatString, "svm", 3) == 0)
    {
        OutputFormat = PseResultFormat::SVM;
        return;
    }
    if (strncasecmp(OutputFormatString, "tsv", 3) == 0)
    {
        OutputFormat = PseResultFormat::TSV;
        return;
    }
    if (strncasecmp(OutputFormatString, "csv", 3) == 0)
    {
        OutputFormat = PseResultFormat::CSV;
        return;
    }
    ErrorAbort(ErrorCode::UndefinedOutputFormat);
}

void UltraPse::PseTask::SetNotationName(const char *n)
{
    NotationName = strupr(strdup(n));
}

const char *UltraPse::PseTask::GetNotationName()
{
    return NotationName;
}

PseResultFormat UltraPse::PseTask::GetResultFormat()
{
    return OutputFormat;
}

vector<const char *> *UltraPse::PseTask::GetModNames()
{
    return ModeList;
}

vector<const char *> *UltraPse::PseTask::GetPropNames()
{
    return PropList;
}

map<const char *, const char *, StrKMC> *UltraPse::PseTask::GetParas()
{
    return ExtraPara;
}

bool UltraPse::PseTask::GetValidate()
{
    return AutoValidate;
}

void UltraPse::PseTask::SetValidate(bool v)
{
    AutoValidate = v;
}

