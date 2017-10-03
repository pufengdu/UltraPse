/****************************************************************************
 * RawProperty.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                       *
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

#include "RawProperty.h"
#include "BioNotations.h"

using namespace std;
using namespace UltraPse;

map<const char*, RawProperty *, StrKMC> UltraPse::RawPropLib(StrKeyMapComp);

bool UltraPse::RawProperty::ValidateRawValues(UltraPse::BioNotations *b)
{
    int n = b->GetNotationLength();
    for (auto &p : RawValues)
    {
        const char *t_note = (const char *)p.first;
        int t_noteLen = strlen(t_note);
        if (t_noteLen != n)
            return false;
        for (const char *t = t_note ; *t ; t++)
        {
            if (!b->IsBaseNotation(*t))
                return false;
        }
    }
    return true;
}

PCProperty *UltraPse::RawProperty::Convert(UltraPse::BioNotations *b, bool AutoFill, UltraPse::PCProperty *TemplateProp)
{
    if (!ValidateRawValues(b))
        return 0;
    if (!TemplateProp)
    {
        //No Template
        if (!AutoFill)
        {
            //Check integrity
            if (RawValues.size() != (unsigned int)(b->GetTotalNotations()))
                return 0;
        }
        //If reaches here, either AutoFill is on, or integrity is guaranteed
        PCProperty *r = new PCProperty(b);
        for (auto &p : RawValues)
        {
            char *k = (char*)p.first;
            float v = (float)p.second;
            r->SetValue(k,v);
        }
        return r;
    }
    else
    {
        //If reaches here, Only need to modify the Template
        for (auto &p : RawValues)
        {
            char *k = (char*)p.first;
            float v = (float)p.second;
            TemplateProp->SetValue(k,v);
        }
        return TemplateProp;
    }
}
