/****************************************************************************
 * BioProcessor.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                      *
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

#include "Common.h"
#include "BioProcessor.h"
#include "Operator.h"

using namespace UltraPse;
using namespace std;

UltraPse::BioSequenceList::BioSequenceList()
{
    SeqList = new vector<BioSequence *>();
    ValidateSequence = false;
}

UltraPse::BioSequenceList::~BioSequenceList()
{
    SelfClear();
}

void UltraPse::BioSequenceList::SetValidOption(bool v)
{
    ValidateSequence = v;
}


void UltraPse::BioSequenceList::SelfClear()
{
    for (BioSequence *&p : *SeqList)
    {
        delete p;
        p = 0;
    }
    delete SeqList;
    SeqList = 0;
}

vector<BioSequence *> *UltraPse::BioSequenceList::GetList()
{
    return SeqList;
}

void UltraPse::BioSequenceList::SequenceFilter(BioSequence *s)
{
    if (ValidateSequence)
    {
        s->ValidateSequence();
    }
    s->ConvertToIndex();
    SeqList->push_back(s);
}

void UltraPse::BioSequenceList::CloseBatch()
{
    //Send current batch to PseEngine, Output/Store result.
    //op->BatchNotify();

    /*for (BioSequence *&p : *SeqList)
    {
        printf(">%s\n",p->GetID());

        printf("%s\n",p->GetSequenceClone());
    }*/
    //op->SetSeqList(this);
    op->BatchNotify();

    //Release sequences
    SelfClear();
    //Re-creat batch storage
    SeqList = new vector<BioSequence *>();
}

void UltraPse::BioSequenceList::SetOperator(Operator *t)
{
    op = t;
}
