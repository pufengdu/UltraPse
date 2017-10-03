/****************************************************************************
 * PCLib.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                             *
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

#include "PCLib.h"
#include "BioNotations.h"
#include "StdProt.h"
#include "StdDNA.h"
#include "StdRNA.h"

using namespace UltraPse;
using namespace std;

std::map<const char *, unsigned char *, StrKMC> UltraPse::PropertyLibRegistry
(
    {
        {"STDPROT", UltraPse::StdProtProps},
        {"DIDNA", UltraPse::DiDNAProps},
        {"DIRNA", UltraPse::DiRNAProps},
        {"TRIDNA", UltraPse::TriDNAProps}
    },
    UltraPse::StrKeyMapComp
);

UltraPse::PCLibrary::PCLibrary(Byte *data_block, BioNotations* b)
{
    int *t_vals = (int*)data_block;
    int cnt_props = *t_vals; t_vals ++;
    int cnt_total_bytes = *t_vals; t_vals ++;
    int cnt_BioNotations = *t_vals; t_vals ++;
    int lenID = *t_vals; t_vals ++;
    int lenValue = *t_vals; t_vals ++;
    //int lenComment = *t_vals;
    t_vals ++;
    int validate_cnt_BioNotations = b->GetTotalNotations();
    if (validate_cnt_BioNotations != cnt_BioNotations)
    {
        ErrorAbort(ErrorCode::PCLibInitError);
    }
    h = data_block;
    IndexBlock = h + sizeof(int) * 6;
    IDBlock = IndexBlock + sizeof(PropertyIndex) * cnt_props;
    ValueBlock = IDBlock + lenID;
    CommentBlock = ValueBlock + lenValue;
    Assoc_Note = b;
    count_props = cnt_props;
    count_bytes = cnt_total_bytes;
    StrKMC t_ptr = StrKeyMapComp;
    PCIndex = new map<const char *, PropertyIndex*, StrKMC>(t_ptr);
    ActiveProperties = new vector<PCProperty *>();
    PropertyIndex *t = (PropertyIndex*)IndexBlock;
    for (int i = 0 ; i < count_props ; i++)
    {
        const char *t_id = (const char *)(IDBlock + t->id_offset);
        (*PCIndex)[t_id] = t;
        t ++;
    }
}

UltraPse::PCLibrary::~PCLibrary()
{
    delete PCIndex;
    delete ActiveProperties;
}

PCProperty *UltraPse::PCLibrary::GetProperty(const char *id)
{
    PropertyIndex *t_index = (*PCIndex)[id];
    // This zero indicates that the required property is user defined, and has already been added to the active set.
    if (!t_index) return 0;

    //Otherwise, this is a library property, create property object, add it to active set, return it.
    PCProperty *r = new PCProperty(Assoc_Note);
    r->CopyValues((float *)(ValueBlock + (*PCIndex)[id]->value_offset));
    r->SetID((const char *)(IDBlock + (*PCIndex)[id]->id_offset));
    r->SetComment((const char*)(CommentBlock + (*PCIndex)[id]->comment_offset));
    r->PreCompute();
    ActiveProperties->push_back(r);
    return r;
}

std::vector<PCProperty *> *UltraPse::PCLibrary::GetActiveProperties()
{
    return ActiveProperties;
}

void UltraPse::PCLibrary::AddUserProperty(PCProperty *s)
{
    (*PCIndex)[s->GetID()] = 0;
    ActiveProperties->push_back(s);
}

void UltraPse::PCLibrary::ListProperty()
{
    int k = 0;
    for (auto &p : *PCIndex)
    {
        PropertyIndex *t = static_cast<PropertyIndex*>(p.second);
        char *id = (char*)(IDBlock + t->id_offset);
        char *comment = (char*)(CommentBlock + t->comment_offset);
        printf("%s\t%s\n",id,comment);
        k ++;
    }
    printf("Total number of properties : %d\n", k);
}
