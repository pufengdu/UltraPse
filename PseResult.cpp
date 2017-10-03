/****************************************************************************
 * PseResult.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                         *
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

#include "PseResult.h"
#include <cstdio>
using namespace UltraPse;

UltraPse::PseResult::PseResult(BioSequence *p)
{
    this->Assoc_Seq = p;
    PseSerial = 0;
    Length = 0;

}

UltraPse::PseResult::~PseResult()
{
    if (PseSerial)
    {
        delete [] PseSerial;
        PseSerial = 0;
    }
    Length = 0;
}

float *UltraPse::PseResult::InitResult(int l)
{
    PseSerial = new float[l];
    memset(PseSerial, 0, sizeof(float) * l);
    Length = l;
    return PseSerial;
}

float *UltraPse::PseResult::GetSerial()
{
    return PseSerial;
}

int UltraPse::PseResult::GetLength()
{
    return Length;
}

void UltraPse::PseResult::DisplayCSV(FILE *fh)
{
    fprintf(fh, "%s", Assoc_Seq->GetID());
    for (int i = 0 ; i < Length ; i ++)
    {
        fprintf(fh, ", %.3f",PseSerial[i]);
    }
    fprintf(fh, ", %s\n", Assoc_Seq->GetComment());
}

void UltraPse::PseResult::DisplayTSV(FILE *fh)
{
    fprintf(fh, "%s", Assoc_Seq->GetID());
    for (int i = 0 ; i < Length ; i ++)
    {
        fprintf(fh, "\t%.3f",PseSerial[i]);
    }
    fprintf(fh, "\t%s\n", Assoc_Seq->GetComment());
}

void UltraPse::PseResult::DisplaySVM(FILE *fh)
{
    fprintf(fh, "%d ", 0);
    for (int i = 0; i < Length; i ++)
    {
        fprintf(fh, " %d:%.3f", i + 1, PseSerial[i]);
    }
    fprintf(fh, " # %s\n", Assoc_Seq->GetID());
}

void UltraPse::DisplayCSV(FILE *fh, UltraPse::PseResult *r)
{
    r->DisplayCSV(fh);
}

void UltraPse::DisplayTSV(FILE *fh, UltraPse::PseResult *r)
{
    r->DisplayTSV(fh);
}

void UltraPse::DisplaySVM(FILE *fh, UltraPse::PseResult *r)
{
    r->DisplaySVM(fh);
}

namespace UltraPse
{
    void (*DisplayFuncs[])(FILE *, PseResult *) =
    {
        DisplayCSV,
        DisplayTSV,
        DisplaySVM
    };
    void (*DisplayResult)(FILE *, PseResult *) = DisplayCSV;
};

void UltraPse::SetupDisplayHandle(UltraPse::PseResultFormat t)
{
    UltraPse::DisplayResult = DisplayFuncs[t];
}
