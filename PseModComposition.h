/****************************************************************************
 * PseModComposition.h - Copyright 2017 Pu-Feng Du, Ph.D.                   *
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

#ifndef PSEMODCOMPOSITION_H_INCLUDED
#define PSEMODCOMPOSITION_H_INCLUDED

namespace UltraPse
{
    class PseModComposition : public PseModule
    {
        public:
            PseModComposition(const char *n);
            ~PseModComposition();
            int GetLength();
            float *Compute(int *IndexSerial, int LenghtOfIndex, float *r);
            void Config();
            static PseModule *Init(const char *n);
        private:
            int Length;
    };
}



#endif // PSEMODCOMPOSITION_H_INCLUDED
