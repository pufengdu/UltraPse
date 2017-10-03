/****************************************************************************
 * PseModuleRaw.h - Copyright 2017 Pu-Feng Du, Ph.D.                        *
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

#include "PseModule.h"
#ifndef PSEMODULERAW_H_INCLUDED
#define PSEMODULERAW_H_INCLUDED

namespace UltraPse
{
    //Abstract computation module for GO/FunD/PSSM or any other mode that relies on sequence alignments and IDs.
    class PseModuleRaw : public PseModule
    {
        public:
            PseModuleRaw(const char *n) : PseModule(n)
            {
                SeqClone = 0;
                SeqID = 0;
                SetRaw(true);
            }
            ~PseModuleRaw() {}
            char *GetSeqClone() { return SeqClone;}
            char *GetSeqID() {return SeqID;}
            void SetSeqClone(char *s) {SeqClone = s;}
            void SetSeqID(char *s) {SeqID = s;}
            // The common interface for all computing modules in PseEngine.
            // Config the module according to the parameters and properties.
            virtual void Config() = 0;
            // Return the CONFIGURED length of computing results BEFORE actual computations
            virtual int GetLength() = 0;
            // The Compute function MUST return a pointer just after the last element, r is the place to write all results.
            virtual float *Compute(int *IndexSerial, int LenghtOfIndex, float *r) = 0;
        private:
            char *SeqClone;
            char *SeqID;
    };
};


#endif // PSEMODULERAW_H_INCLUDED
