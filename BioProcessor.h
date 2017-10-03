/****************************************************************************
 * BioProcessor.h - Copyright 2017 Pu-Feng Du, Ph.D.                        *
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
//#include "Operator.h"

#ifndef BIOPROCESSOR_H_INCLUDED
#define BIOPROCESSOR_H_INCLUDED

namespace UltraPse
{
    //Abstract interface for the FastaParser and the PseEngine
    //Inherit this class to do various operations on BioSequence objects
    class BioProcessor
    {
        public:
            BioProcessor(){};
            virtual ~BioProcessor(){};
            virtual void SequenceFilter(BioSequence *s) = 0;
            virtual void CloseBatch() = 0;
    };

    class Operator;

    class BioSequenceList: public BioProcessor
    {
        public:
            BioSequenceList();
            ~BioSequenceList();
            void SequenceFilter(BioSequence *s);
            void CloseBatch();
            void SelfClear();
            void SetOperator(Operator *t);
            std::vector<BioSequence *> *GetList();

            void SetValidOption(bool v);

        private:
            std::vector<BioSequence *> *SeqList;
            Operator *op;
            bool ValidateSequence;
    };
};

#endif // BIOPROCESSOR_H_INCLUDED
