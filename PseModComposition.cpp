/****************************************************************************
 * PseModComposition.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                 *
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

#include "PseModComposition.h"

using namespace UltraPse;

PseModule *UltraPse::PseModComposition::Init(const char *n)
{
    return new PseModComposition(n);
}

UltraPse::PseModComposition::PseModComposition(const char *n) : PseModule(n)
{

}

UltraPse::PseModComposition::~PseModComposition()
{

}

int UltraPse::PseModComposition::GetLength()
{
    return Length;
}

void UltraPse::PseModComposition::Config()
{
    BioNotations *bn = GetNotations();
    Length = bn->GetTotalNotations();
}

float *UltraPse::PseModComposition::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    for (int i = 0 ; i < LengthOfIndex ; i ++)
    {
        r[IndexSerial[i]] ++;
    }
    for (int i = 0 ; i < Length ; i ++)
    {
        r[i] /= LengthOfIndex;
        r[i] *= 100;
    }
    return r + Length;
}

