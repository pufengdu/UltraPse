/****************************************************************************
 * BioSequence.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                       *
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

#include "BioSequence.h"

using namespace UltraPse;
using namespace std;

//Pre-defined Fasta db type prefix
//ADD NEW DB_TYPES BEFORE NIL!!
//REMEMBER TO MODIFY FastaDbFormatType ENUM ALSO!!
UltraPse::FastaDBType UltraPse::DB_TYPES[] =
{
    {FastaDbFormatType::GBD,    "gb"},
    {FastaDbFormatType::EMB,    "emb"},
    {FastaDbFormatType::DBJ,    "dbj"},
    {FastaDbFormatType::SPD,    "sp"},
    {FastaDbFormatType::REF,    "ref"},
    {FastaDbFormatType::NIL,    ""}
};

UltraPse::BioSequence::BioSequence(char *CommentLine)
{
    q = 0;
    cq = 0;
    Len_Seq = 0;
    NoteIndex = 0;
    Len_Index = 0;
    BreakBuffer = 0;
    Assoc_Note = 0;
    Id = 0;
    Comment = 0;
    Len_Id = 0;
    Len_Comment = 0;
    ParseCommentLine(CommentLine);
    PartBuffer = new vector<SeqPart*>();
    Sealed = false;
    ClonedSequence = 0;
    isValid = true;
}

UltraPse::BioSequence::~BioSequence()
{
    if (PartBuffer)
    {
        for (SeqPart *t : *PartBuffer)
        {
            delete t;
            t = 0;
        }
        delete PartBuffer;
        PartBuffer = 0;
    }

    if (q)
    {
        for (int i = 0 ; i < cq ; i ++)
        {
            delete q[i];
            q[i] = 0;
        }
        delete [] q;
        q = 0;
    }
    delete [] BreakBuffer;
    BreakBuffer = 0;

    delete [] NoteIndex;
    NoteIndex = 0;

    delete [] ClonedSequence;
    ClonedSequence = 0;
}

char *UltraPse::BioSequence::GetComment()
{
    return this->Comment;
}

char *UltraPse::BioSequence::GetID()
{
    return this->Id;
}

void UltraPse::BioSequence::SetNotation(BioNotations *s)
{
    Assoc_Note = s;
    BreakBuffer = new char[(Assoc_Note->GetNotationLength() - 1) * 2];
}

UltraPse::FastaDbFormatType UltraPse::BioSequence::GetDbType(char *s)
{
    FastaDBType *t = DB_TYPES;
    while (t->db_type_id != FastaDbFormatType::NIL)
    {
        if (CheckPrefix(s,t->prefix))
            break;
        t ++;
    }
    return t->db_type_id;
}

int *UltraPse::BioSequence::GetIndexSerial()
{
    return NoteIndex;
}

int UltraPse::BioSequence::GetNumberOfIndex()
{
    return Len_Index;
}

void UltraPse::BioSequence::ParseCommentLine(char *s)
{
    FastaDbFormatType db_type = GetDbType(s);
    if (db_type == FastaDbFormatType::NIL)
    {
        Id = s;
        Len_Id = strlen(s);
        Comment = 0;
        Len_Comment = 0;
        return;
    }
    int cnt_sep = 0;
    char *t = s;
    char *b = 0;
    while (*t)
    {
        if (*t == FastaCommentSep)
        {
            cnt_sep ++;
            if (cnt_sep == 2)
            {
                b = t;
                break;
            }
        }
        t ++;
    }
    if (b)
    {
        *b = 0;
        b++;
    }

    Id = s;
    Len_Id = strlen(s);

    Comment = b;
    Len_Comment = b ? strlen(b) : 0;
}

void UltraPse::BioSequence::AddSeqPart(char *s, int l)
{
    SeqPart *t = new SeqPart;
    t->l = l;
    t->s = s;
    PartBuffer->push_back(t);
    Len_Seq += l;
}

void UltraPse::BioSequence::SealSeqPart()
{
    cq = PartBuffer->size();
    q = new SeqPart*[cq];
    int i = 0;
    for (SeqPart *t : *PartBuffer)
    {
        q[i]  = t;
        i++;
    }
    delete PartBuffer;
    PartBuffer = 0;
    Sealed = true;
}

char *UltraPse::BioSequence::GetSequenceClone()
{
    char *r = new char[Len_Seq + 1];
    memset(r,0,sizeof(char) * (Len_Seq + 1));
    int copy_offset = 0;
    for (int i = 0 ; i < cq ; i ++)
    {
        memcpy(r + copy_offset, q[i]->s, q[i]->l * sizeof(char));
        copy_offset += q[i]->l;
    }

    if (ClonedSequence)
        delete [] ClonedSequence;
    ClonedSequence = r;
    return r;
}

void UltraPse::BioSequence::ConvertToIndex()
{
    int Len_Note = Assoc_Note->GetNotationLength();
    Len_Index = Len_Seq - Len_Note + 1;
    NoteIndex = new int[Len_Index];
    memset(NoteIndex, 0, sizeof(int) * Len_Index);

    //Invalid sequence / sequence not conforming notation defs, leave blank indexes
    if (!isValid)
        return;

    //Convert line by line, leaving line breaks!
    int offset_index = 0;

    for (int i = 0 ; i < cq; i ++)
    {
        Assoc_Note->ConvertSequenceToIndex(q[i]->s, NoteIndex + offset_index, q[i]->l);
        offset_index += q[i] -> l;
    }

    //When Len_Note is 1, there is no line break problem.
    if (Len_Note == 1)
        return;

    // Fill the line breaks, totally cq - 1 breaks!!
    // The last break must be dealt with specially!
    bool last_skip = (q[cq - 1]->l < Len_Note);

    offset_index = 0;
    for (int i = 0 ; i < cq - last_skip - 1; i ++)
    {
        offset_index += q[i]->l;
        //Construct break buffer
        for (int j = 0; j < Len_Note - 1 ; j ++)
            BreakBuffer[j] = (q[i]->s)[q[i]->l - Len_Note + 1 + j];
        for (int j = 0; j < Len_Note - 1; j ++)
            BreakBuffer[Len_Note - 1 + j] = (q[i+1]->s)[j];
        Assoc_Note->ConvertSequenceToIndex(BreakBuffer, NoteIndex + offset_index - Len_Note + 1, (Len_Note - 1) * 2);
    }

    //The last line break, caution!!
    if (last_skip)
    {
        for (int j = 0; j < Len_Note - 1; j ++)
            BreakBuffer[j] = (q[cq - 2]->s)[q[cq - 2]->l - Len_Note + 1 + j];
        for (int j = 0; j < q[cq - 1] -> l ; j ++)
            BreakBuffer[Len_Note - 1 + j] = q[cq - 1]->s[j];
        int t_offset = Len_Seq - q[cq - 1]->l - Len_Note + 1;
        Assoc_Note->ConvertSequenceToIndex(BreakBuffer, NoteIndex + t_offset, Len_Note - 1 + q[cq - 1]->l);
    }
}

void UltraPse::BioSequence::ValidateSequence()
{
    for (int i = 0 ; i < cq ; i ++)
    {
        if (!Assoc_Note->ValidateSequence(q[i]->s, q[i]->l))
        {
            isValid = false;
            return;
        }
    }
}

bool UltraPse::BioSequence::IsValid()
{
    return isValid;
}
