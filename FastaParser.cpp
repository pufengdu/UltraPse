/****************************************************************************
 * FastaParser.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                       *
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

#include "FastaParser.h"

using namespace UltraPse;
using namespace std;

UltraPse::FastaParser::FastaParser(BioNotations *b, BioProcessor *p)
{
    Assoc_Note = b;
    Assoc_Proc = p;
    Current_Sequence = 0;
    State = FastaParserState::Ready;
}

void UltraPse::FastaParser::Reset()
{
    Assoc_Proc->CloseBatch();
    delete Current_Sequence;
    Current_Sequence = 0;
    State = FastaParserState::Ready;
    //Current_Signal = FastaParserSignal::Release;
}

UltraPse::FastaParser::~FastaParser()
{

}

UltraPse::FastaParserState UltraPse::FastaParser::GetState()
{
    return State;
}

void UltraPse::FastaParser::SetState(char s)
{
    switch (s)
    {
        case '>':
            if (State == FastaParserState::Ready)
                State = FastaParserState::Start;
            else
                State = FastaParserState::CommentLine;
            Current_Signal = FastaParserSignal::Refresh;
            break;
        default:
            State = FastaParserState::SequenceLine;
            Current_Signal = FastaParserSignal::Keep;
            break;
    };
}

void UltraPse::FastaParser::Dispatch(char *Line)
{
    switch(State)
    {
        case FastaParserState::Ready:
            break;
        case FastaParserState::Start:
            CreateNextRecord(Line);
            break;
        case FastaParserState::CommentLine:
            ProcLastRecord();
            CreateNextRecord(Line);
            break;
        case FastaParserState::SequenceLine:
            Current_Sequence->AddSeqPart(Line, strlen(Line));
            break;
        case FastaParserState::Terminate:
            ProcLastRecord();
            break;
    }
}

void UltraPse::FastaParser::Transfer(char *Line)
{
    SetState(Line[0]);
    Dispatch(Line);
}

void UltraPse::FastaParser::Finish()
{
    State = FastaParserState::Terminate;
    Dispatch(0);
    Assoc_Proc->CloseBatch();
    Current_Signal = FastaParserSignal::Release;
}

void UltraPse::FastaParser::ProcLastRecord()
{
    Current_Sequence->SealSeqPart();
    Assoc_Proc->SequenceFilter(Current_Sequence);

    /*if (!Assoc_Proc->SequenceFilter(Current_Sequence))
    {
        delete Current_Sequence;
        Current_Sequence = 0;
    }*/
}

void UltraPse::FastaParser::CreateNextRecord(char *Line)
{
    Current_Sequence = new BioSequence(Line + 1);
    Current_Sequence->SetNotation(Assoc_Note);
}

UltraPse::FastaParserSignal UltraPse::FastaParser::GetSignal()
{
    return Current_Signal;
}
