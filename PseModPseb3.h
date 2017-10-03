/****************************************************************************
 * PseModPseb3.h - Copyright 2017 Pu-Feng Du, Ph.D.                         *
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

#include <lua.hpp>
#include "PseModuleRaw.h"

#ifndef PSEMODPSEB3_H_INCLUDED
#define PSEMODPSEB3_H_INCLUDED

namespace UltraPse
{
    int GetPCList(lua_State *L);
    int GetOptionList(lua_State *L);

    class PseModPseb3 : public PseModuleRaw
    {
        public:
            PseModPseb3(const char *n);
            ~PseModPseb3();
            // The common interface for all computing modules in PseEngine.
            // Config the module according to the parameters and properties.
            void Config();
            // Return the CONFIGURED length of computing results BEFORE actual computations
            int GetLength();
            // The Compute function MUST return a pointer just after the last element, r is the place to write all results.
            float *Compute(int *IndexSerial, int LenghtOfIndex, float *r);
            static PseModule *Init(const char *n);
        private:
            int Length;
    };
};

#endif // PSEMODPSEB3_H_INCLUDED
