/****************************************************************************
 * BioNotations.h - Copyright 2017 Pu-Feng Du, Ph.D.                        *
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
#ifndef BIONOTATIONS_H
#define BIONOTATIONS_H
namespace UltraPse
{
    //Number of all possible printable notations in ASCII table
    extern const char FirstPrintableChar;
    extern const char LastPrintableChar;
    extern const int count_of_letters;
    extern const char *DefaultNotationName;


 /**
    Some notes about BioNotations class:
    1. BioNotations class defines general framework for a notations definitions
    Here, notations include a set of base symbols and the length of symbols that forms a notation
    For example, the di-nucleotide DNA Notations can be defined as BioNotations("ACGT",2, true)
    For example, the tri-nucleodite RNA Notations can be defined as BioNotations("ACGU", 3, true)

    2. The reduced rules means, you can let several notations to be treated equally.
    For example, for the di-nucleotide DNA, the reducing rules can be written like
    "ATAAGC", which means AT=AA=GC.
    For another example, for the protein sequences, the reducing rules can be written
    like "KLT", which means K=L=T.
    For multiple reducing rules, they should be seperated by ';', no other spaces and terminals
    like "ATAAGC;GTGAGG"

    3. The BioNotation class is responsible to convert sequence to index numbers. The
    following methods can be used:
    int ConNotationToIndex(char *s);  //Use multiplications in conversion
    int ConNotationToIndex(int *s);
    int MapNotationToIndex(char *s);  // Use pre-computed indexes in conversion
    int MapNotationToIndex(int *s);
    The "Conxxxxxxx" methods compute the index numbers using multiplication operations. It can be
    performed on BioNotations objects without Mapping facility (3rd parameter is false). It is slower,
    but slightly more memory efficient.
    The "Mapxxxxxxx" methods use Mapping facility to convert notations to index numbers. It require
    a pre-computed map in the object. It is faster, but needs slightly more memory.
    The parameter int *s means the sequences has been preprocessed, converted to single-byte index
    The parameter char *s means this is the sequence is self.



 **/

    //Define a set of notation rules:
    //Single-byte notation set
    //Length of multi-byte notations
    //Reducing rules
    //Single-byte notation <-> index conversion order
    //Provides two method to convert multi-byte notations to indexes, from char * / int *
    //Convert single-byte notation to its order
    //Using the pre-computed mapping, constant number of multiplications will be performed.

    class BioNotations
    {
        public:
            BioNotations(const char *b, int note_length, bool useMap);
            int ConNotationToIndex(char *s); //Use multiplications in conversion, without reducing!!
            int ConNotationToIndex(int *s);
            int MapNotationToIndex(char *s);    // Use pre-computed indexes in conversion, reduced!!
            int MapNotationToIndex(int *s);
            int GetNotationToIndex(char s); //Convert a single-byte note to its index
            bool IsBaseNotation(char t); //Check whether a single-byte is a valid base note

            void AddReduceRule(char *r); //Add a reduce rule to the reduce rule set
            int ReduceIndex(int s); //Convert an original index to reduced index

            void Display(); //Debug only: Display summary of the objects

            int GetTotalNotations();
            int GetTotalBaseNotations();
            int GetNotationLength();

            void IndexToNotation(int n, char *r); //TODO: convert index to notation

            virtual ~BioNotations();

            static BioNotations* GetNotationFromLib(const char *NoteName);
            static void DisplayLibrary();

            void ConvertSequenceToIndex(char *s, int *r, int l);
            bool ValidateSequence(char *s, int l);

        private:
            char *BaseNotations;    //All single-byte notations in a string
            int BaseCount;  //Number of BaseNotations;
            int LengthOfNotation;   //Length of every mbNotations
            char ZeroNote;  //The single-byte notation that equal to zero, usually ' '
            int *ReduceRule;    //Reducing notation rule sets
            bool IsReduceRuleExist; //Whether there are reduced rules
            int *BaseNoteMap;   //Mapping the Base Notations to indexes
            int *MBNoteMap; //Mapping the multi-byte notations;
            bool IsMultiByteNotes;  //Whether current notation definition is a multi-byte mapping
            bool IsMultiByteMapCreated;
            void CreateBaseMap();   //Create single-byte notation mapping
            void CreateMBNoteMap(); //Create multi-byte notation mapping
            void CreateReduceRules();

            //char **AllNotations; // TODO: For function IndexToNotation

    };

    struct BioNoteDef
    {
        const char *Name;
        const char *BaseNotes;
        int Len;
        bool UseMap;
        const char *ReduceRule;
    };

    extern std::map<const char*, BioNoteDef, StrKMC> BioNoteDefLib;

};

#endif // BIONOTATIONS_H
