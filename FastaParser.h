/****************************************************************************
 * FastaParser.h - Copyright 2017 Pu-Feng Du, Ph.D.                         *
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

#include "BioSequence.h"
#include "BioProcessor.h"

#ifndef FASTAPARSER_H_INCLUDED
#define FASTAPARSER_H_INCLUDED

namespace UltraPse
{
    //Fasta format lines parsed as FSM
    enum FastaParserState {Ready, Start, CommentLine, SequenceLine, Terminate};
    enum FastaParserSignal {Refresh, Keep, Release};

    class FastaParser
    {
        public:
            FastaParser(BioNotations *b, BioProcessor *p);
            ~FastaParser();
            FastaParserState GetState();
            void SetState(char s);
            void Transfer(char *Line);
            void Dispatch(char *Line);
            void Finish();
            FastaParserSignal GetSignal();
            void Reset();

        private:
            void ProcLastRecord();
            void CreateNextRecord(char *Line);
            FastaParserState State;
            FastaParserSignal Current_Signal;
            BioNotations *Assoc_Note;
            BioProcessor *Assoc_Proc;
            BioSequence *Current_Sequence;
    };
};

#endif // FASTAPARSER_H_INCLUDED
