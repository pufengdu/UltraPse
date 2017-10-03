/****************************************************************************
 * PseModPseAAC.h - Copyright 2017 Pu-Feng Du, Ph.D.                        *
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
#include "Common.h"

#ifndef PSEMODPSEAAC_H_INCLUDED
#define PSEMODPSEAAC_H_INCLUDED

namespace UltraPse
{
    class PseModPseAAC : public PseModule
    {
        public:
            PseModPseAAC(const char *n);
            ~PseModPseAAC();
            int GetLength();
            float *Compute(int *IndexSerial, int LenghtOfIndex, float *r);
            void Config();
            static PseModule *Init(const char *n);
        private:
            float *ComputeType1(int *IndexSerial, int LenghtOfIndex, float *r);
            float *ComputeType2(int *IndexSerial, int LenghtOfIndex, float *r);
            float *ComputeAAC(int *IndexSerial, int LenghtOfIndex, float *r);
            int Length;
            int SubType;
            int Lambda;
            float Omega;
            float ***ActiveMatrix;
            int LengthAAC;
            int CountProps;
    };
};

#endif // PSEMODPSEAAC_H_INCLUDED
