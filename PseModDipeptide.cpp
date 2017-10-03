/****************************************************************************
 * PseModDipeptide.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                   *
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

#include "PseModDipeptide.h"

using namespace UltraPse;

PseModule *UltraPse::PseModDipeptide::Init(const char *n)
{
    return new PseModDipeptide(n);
}

UltraPse::PseModDipeptide::PseModDipeptide(const char *n): PseModule(n)
{
    CountOfAA = 20;
    Length = CountOfAA * CountOfAA;
}

UltraPse::PseModDipeptide::~PseModDipeptide()
{

}

void UltraPse::PseModDipeptide::Config()
{

}

int UltraPse::PseModDipeptide::GetLength()
{
    BioNotations *bn = GetNotations();
    int cnt_base = bn->GetTotalBaseNotations();
    int note_len = bn->GetNotationLength();
    if (cnt_base != CountOfAA || note_len != 1)
    {
        ErrorAbort(ErrorCode::InvalideNotations);
        return 0;
    }
    else
    {
        return Length;
    }
}

float *UltraPse::PseModDipeptide::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    float **t = new float*[CountOfAA];
    t[0] = r;
    for (int i = 1 ; i < CountOfAA ; i ++)
    {
        t[i] = t[i - 1] + CountOfAA;
    }
    for (int i = 0; i < LengthOfIndex - 1; i ++)
    {
        t[IndexSerial[i]][IndexSerial[i+1]] ++;
    }
    for (int i = 0; i < Length; i ++)
    {
        r[i] /= (LengthOfIndex - 1);
        r[i] *= 100;
    }
    delete [] t;
    return r + Length;
}
