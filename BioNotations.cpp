/****************************************************************************
 * BioNotations.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                      *
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

#include "BioNotations.h"
#include "Common.h"
using namespace UltraPse;
using namespace std;

const char UltraPse::FirstPrintableChar = 0x20;
const char UltraPse::LastPrintableChar = 0x7F;
const int UltraPse::count_of_letters = LastPrintableChar - FirstPrintableChar;
const char *UltraPse::DefaultNotationName = "STDPROT";

//Pre-defined BioNoteDefinitions
map<const char *, BioNoteDef, StrKMC> UltraPse::BioNoteDefLib
(
    {
        {"STDDNA",  {"STDDNA",  "ATGC", 1,  true, 0}},
        {"DIDNA",   {"DIDNA",   "ATGC", 2,  true, 0}},
        {"TRIDNA",  {"TRIDNA",  "ATGC", 3,  true, 0}},
        {"STDRNA",  {"STDRNA",  "AUGC", 1,  true, 0}},
        {"DIRNA",   {"DIRNA",   "AUGC", 2,  true, 0}},
        {"TRIRNA",  {"TRIRNA",  "AUGC", 3,  true, 0}},
        {"STDPROT", {"STDPROT", "ACDEFGHIKLMNPQRSTVWY", 1,  true, 0}}
    },
    UltraPse::StrKeyMapComp
);

UltraPse::BioNotations::BioNotations(const char *b, int note_length, bool useMap)
{
    if (note_length <= 0)
    {
        ErrorAbort(ErrorCode::InvalideNotations);
    }

    //Copy base notation definitions
    BaseCount = strlen(b);  //Radix
    BaseNotations = new char[BaseCount + 1];
    for (int i = 0 ; i < BaseCount + 1; i++)
        //BaseNotations[i] = toupper(b[i]);
        BaseNotations[i] = b[i];

    //Sort the BaseNotations, so that all base indexes are according to alphabetical order.
    //Independent to the appearance.
    qsort(BaseNotations, BaseCount, sizeof(char), qSortComp<char>);

    //Define the character set
    ZeroNote = FirstPrintableChar;

    BaseNoteMap = new int[count_of_letters];
    for (int i = 0 ; i < count_of_letters; i++)
        BaseNoteMap[i] = -1;
    LengthOfNotation = note_length; //Digits
    CreateBaseMap();

    IsMultiByteNotes = (LengthOfNotation > 1);

    MBNoteMap = 0;
    IsMultiByteMapCreated = false;

    if (useMap && IsMultiByteNotes)
    {
        CreateMBNoteMap();
        IsMultiByteMapCreated = true;
    }

    CreateReduceRules();
    IsReduceRuleExist = false;
}

UltraPse::BioNotations::~BioNotations()
{
    delete [] BaseNotations;
    delete [] BaseNoteMap;
    delete [] MBNoteMap;
    delete [] ReduceRule;
}

void UltraPse::BioNotations::IndexToNotation(int n, char *r)
{
    for (int i = 0 ; i < LengthOfNotation ; i++)
    {
        int tr = n % BaseCount;
        r[LengthOfNotation - 1 - i] = BaseNotations[tr];
        n = (n - tr) / BaseCount;
    }
}

void UltraPse::BioNotations::CreateBaseMap()
{
    for (char *p = BaseNotations; *p ; p ++)
    {
        //BaseNoteMap[toupper(*p) - ZeroNote] = p - BaseNotations;
        BaseNoteMap[*p - ZeroNote] = p - BaseNotations;
    }
}

void UltraPse::BioNotations::CreateMBNoteMap()
{
    int TotalMBMapSize = 0;
    int FinalMBMapSize = 1;
    int IndexMBMapSize = 0;
    for (int i = 0 ; i < LengthOfNotation ; i++)
    {
        TotalMBMapSize += FinalMBMapSize * BaseCount;
        FinalMBMapSize *= BaseCount;
    }
    MBNoteMap = new int[TotalMBMapSize];
    IndexMBMapSize = TotalMBMapSize - FinalMBMapSize;
    for (int i = 0 ; i < IndexMBMapSize ; i ++)
        MBNoteMap[i] = (i + 1) * BaseCount;
    for (int i = IndexMBMapSize ; i < TotalMBMapSize ; i ++)
        MBNoteMap[i] = i - IndexMBMapSize;
}

int UltraPse::BioNotations::MapNotationToIndex(int *s)
{
    if (!IsMultiByteMapCreated)
    {
        ErrorAbort(ErrorCode::MultiByteMapNotCreated);
    }
    int r = 0;
    for (int i = 0 ; i < LengthOfNotation ; i ++)
        r = MBNoteMap[r + s[i]];
    return IsReduceRuleExist ? ReduceIndex(r) : r;
    //return r;
}

int UltraPse::BioNotations::MapNotationToIndex(char *s)
{
    if (!IsMultiByteMapCreated)
    {
        ErrorAbort(ErrorCode::MultiByteMapNotCreated);
    }
    int r = 0;
    for (int i = 0 ; i < LengthOfNotation ; i ++)
    {
        r = MBNoteMap[r + BaseNoteMap[s[i]-ZeroNote]];
    }
    return IsReduceRuleExist ? ReduceIndex(r) : r;
    //return r;
}

bool UltraPse::BioNotations::IsBaseNotation(char t)
{
    int i = toupper(t) - ZeroNote;
    if (i >=0 && i < count_of_letters)
        if (BaseNoteMap[i] >= 0)
            return true;
    return false;
}

int UltraPse::BioNotations::GetNotationToIndex(char s)
{
    return BaseNoteMap[s - ZeroNote];
}

int UltraPse::BioNotations::ConNotationToIndex(char *s)
{
    int r = 0;
    for (int i = 0 ; i < LengthOfNotation ; i++)
    {
        r = r * BaseCount + BaseNoteMap[s[i] - ZeroNote];
    }
    return r;
}

int UltraPse::BioNotations::ConNotationToIndex(int *s)
{
    int r = 0;
    for (int i = 0 ; i < LengthOfNotation ; i++)
    {
        r = r * BaseCount + s[i];
    }
    return r;
}

void UltraPse::BioNotations::AddReduceRule(char *r)
{
    // The format of reduce rule: "AAATGT",when length of note = 2, means "AA=AT=GT"
    // Prevent some fool problem.
    if (!r)
        return;
    int r_l = strlen(r);
    if (r_l % LengthOfNotation != 0)
        ErrorAbort(ErrorCode::InvalideReducingRule);

    IsReduceRuleExist = true;
    int ReduceIdx = ConNotationToIndex(r);

    //Auto reducing connection by the first reducing notation
    //See if the first notation has already been reduced, if so, reduce to that
    if (ReduceRule[ReduceIdx] != ReduceIdx)
        ReduceIdx = ReduceRule[ReduceIdx];

    //Create the reducing rules
    for (int i = 0 ; i < r_l ; i += LengthOfNotation)
        ReduceRule[ConNotationToIndex(r + i)] = ReduceIdx;
}

void UltraPse::BioNotations::CreateReduceRules()
{
    int ReduceRuleSize = 1;
    for (int i = 0 ; i < LengthOfNotation ; i ++)
        ReduceRuleSize *= BaseCount;
    ReduceRule = new int[ReduceRuleSize];
    for (int i = 0 ; i < ReduceRuleSize ; i ++)
        //ReduceRule[i] = -1;
        ReduceRule[i] = i;
}

int UltraPse::BioNotations::ReduceIndex(int s)
{
    return IsReduceRuleExist ? ReduceRule[s] : s;
}

void UltraPse::BioNotations::Display()
{
    cout << "BioNotations report " << endl;
    cout << "Basic Notations: " << this->BaseNotations << endl;
    cout << "Notation Length: " << this->LengthOfNotation << endl;
    cout << "Using Reduce rules: " << this->IsReduceRuleExist << endl;
}

BioNotations* UltraPse::BioNotations::GetNotationFromLib(const char *NoteName)
{
    if (BioNoteDefLib.find(NoteName) == BioNoteDefLib.end())
    {
        ErrorAbort(ErrorCode::NoSuchNotationType);
    }
    BioNoteDef &t = BioNoteDefLib[NoteName];
    BioNotations *r = new BioNotations(t.BaseNotes, t.Len, t.UseMap);
    char *rr = (char*)t.ReduceRule;
    if (rr)
    {
        char *t_rule = strtok(rr, ";");
        while(t_rule)
        {
            r->AddReduceRule(t_rule);
            t_rule = strtok(0, ";");
        }
    }
    return r;
}

void UltraPse::BioNotations::DisplayLibrary()
{
    for (auto &p : BioNoteDefLib)
    {
        printf("Notation ID: %s\n", p.first);
        printf("=================================================\n");
        printf("\tBaseNotes = %s\n", p.second.BaseNotes);
        printf("\tLength = %d\n", p.second.Len);
        printf("\tReduceRules = %s\n", p.second.ReduceRule);
        printf("=================================================\n");
        printf("\n");
    }
}

int UltraPse::BioNotations::GetTotalNotations()
{
    int r = 1;
    for (int i = 0 ; i < LengthOfNotation ; i ++)
        r *= BaseCount;
    return r;
}

int UltraPse::BioNotations::GetTotalBaseNotations()
{
    return BaseCount;
}

int UltraPse::BioNotations::GetNotationLength()
{
    return LengthOfNotation;
}

void UltraPse::BioNotations::ConvertSequenceToIndex(char *s, int *r, int l)
{
    if (LengthOfNotation == 1)
    {
        for (int i = 0 ; i < l ; i ++)
            r[i] = ReduceIndex(BaseNoteMap[s[i] - ZeroNote]);
    }
    else
    {
        for (int i = 0; i < l - LengthOfNotation + 1 ; i ++)
        {
            r[i] = MapNotationToIndex(s + i);
        }
    }
}

bool UltraPse::BioNotations::ValidateSequence(char *s, int l)
{
    bool r = true;
    for (int i = 0 ; i < l ; i ++)
        r = r && IsBaseNotation(s[i]);
    return r;
}
