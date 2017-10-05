/****************************************************************************
 * PCProperty.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                        *
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

#include "PCProperty.h"

UltraPse::PCProperty::PCProperty(BioNotations *a)
{
    isActive = false;
    Assoc_Note = a;
    int n = Assoc_Note->GetTotalNotations();
    cnt_notes = n;
    ID = 0;
    Comment = 0;
    Values = new float[n];
    float *md = new float[n * n];
    float *mp = new float[n * n];
    memset(Values, 0, sizeof(float) * n);
    memset(md, 0, sizeof(float) * n * n);
    memset(mp, 0, sizeof(float) * n * n);
    DiffMatrix = new float*[n];
    DotMatrix = new float*[n];
    for (int i = 0 ; i < n ; i ++)
    {
        DiffMatrix[i] = md + i * n;
        DotMatrix[i] = mp + i * n;
    }
}

UltraPse::PCProperty::~PCProperty()
{
    delete Values;
    delete DiffMatrix[0];
    delete DotMatrix[0];
    delete DiffMatrix;
    delete DotMatrix;
}

void UltraPse::PCProperty::SetValue(char *Note, float v)
{
    //Values[Assoc_Note->MapNotationToIndex(Note)] = v;
	if (Assoc_Note->GetNotationLength() > 1)
        Values[Assoc_Note->MapNotationToIndex(Note)] = v;
    else
        Values[Assoc_Note->ConNotationToIndex(Note)] = v;
}

void UltraPse::PCProperty::CopyValues(float *s)
{
    memcpy(Values, s, sizeof(float) * cnt_notes);
}

void UltraPse::PCProperty::SetID(const char *d)
{
    ID = d;
}

void UltraPse::PCProperty::SetComment(const char *s)
{
    Comment = s;
}

void UltraPse::PCProperty::PreCompute()
{
    int n = cnt_notes;
    for (int i = 0; i < n; i ++)
    {
        float p = Values[i];
        DotMatrix[i][i] = p * p;
        DiffMatrix[i][i] = 0;
        for (int j = i + 1; j < n; j ++)
        {
            float q = Values[j];
            DotMatrix[j][i] = DotMatrix[i][j] = p * q;
            DiffMatrix[j][i] = DiffMatrix[i][j] = (p - q) * (p - q);
        }
    }
}

float **UltraPse::PCProperty::GetDiffMatrix()
{
    return DiffMatrix;
}

float **UltraPse::PCProperty::GetDotMatrix()
{
    return DotMatrix;
}

const char *UltraPse::PCProperty::GetID()
{
    return this->ID;
}

const char *UltraPse::PCProperty::GetComment()
{
    return this->Comment;
}

UltraPse::PCProperty *UltraPse::PCProperty::SelfClone()
{
    PCProperty *r = new PCProperty(Assoc_Note);
    r->SetID(strdup(ID));
    r->SetComment(strdup(Comment));
    r->CopyValues(Values);
    return r;
}
