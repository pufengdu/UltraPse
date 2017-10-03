/****************************************************************************
 * BioSequence.h - Copyright 2017 Pu-Feng Du, Ph.D.                         *
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

#include "Common.h"
#include "BioNotations.h"

#ifndef BIOSEQUENCE_H
#define BIOSEQUENCE_H

namespace UltraPse
{
    enum FastaDbFormatType {GBD, EMB, DBJ, SPD, REF, NIL};
    struct FastaDBType
    {
        FastaDbFormatType db_type_id;
        const char *prefix;
    };
    extern FastaDBType DB_TYPES[];

    struct SeqPart
    {
        char *s;
        int l;
    };

    class BioSequence
    {
        public:
            BioSequence(char *CommentLine);
            ~BioSequence();

            void SetNotation(BioNotations *s);
            void ConvertToIndex();
            int GetNumberOfIndex();
            int *GetIndexSerial();
            char *GetSequenceClone();

            //Adding and finish adding of all sequence parts
            void AddSeqPart(char *s, int l);
            void SealSeqPart();
            void ParseCommentLine(char *s);

            char *GetID();
            char *GetComment();

            void ValidateSequence();

            bool IsValid();

        private:
            FastaDbFormatType GetDbType(char *s);

            BioNotations *Assoc_Note;
            char *Id;
            char *Comment;
            int Len_Id;
            int Len_Comment;

            SeqPart **q;                        //The actual storage of sequences, in multi-line form
            int cq;                             //Number of lines of multi-line form
            int Len_Seq;                        //Total Length of sequence
            std::vector<SeqPart *> *PartBuffer;   //Intermediate storage for parts

            //Index conversion using Assoc_Note
            int *NoteIndex;
            int Len_Index;
            bool Sealed;
            char *BreakBuffer;

            char *ClonedSequence;

            bool isValid;
    };
};
#endif // BIOSEQUENCE_H
