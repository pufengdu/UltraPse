/****************************************************************************
 * PseModule.h - Copyright 2017 Pu-Feng Du, Ph.D.                           *
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

#include "BioNotations.h"
#include "PCProperty.h"
#include <lua.hpp>

#ifndef PSEMODULE_H_INCLUDED
#define PSEMODULE_H_INCLUDED

namespace UltraPse
{
    //Abstract Module type for computing functions
    class PseModule
    {
        public:
            PseModule(const char *n)
            {
                ModuleName = n;
                Assoc_Note = 0;
                ActiveProps = 0;
                ExtraParas = 0;
                isRaw = false;
            }
            virtual ~PseModule(){}
            bool GetRaw() {return isRaw;}
            void SetRaw(bool t) {isRaw = t;}
            void SetNotations(BioNotations *s) {Assoc_Note = s;}
            BioNotations *GetNotations(){return Assoc_Note;}
            void SetActiveProps(std::vector<PCProperty *> *ps) {ActiveProps = ps;}
            std::vector<PCProperty *> *GetActiveProps() {return ActiveProps;}
            void SetExtraParas(std::map<const char *, const char*, StrKMC> *ep){ ExtraParas = ep;}
            std::map<const char*, const char*, StrKMC> *GetExtraParas(){return ExtraParas;}
            const char *GetName() { return ModuleName;}

            // The common interface for all computing modules in PseEngine.
            // Config the module according to the parameters and properties.
            virtual void Config() = 0;
            // Return the CONFIGURED length of computing results BEFORE actual computations
            virtual int GetLength() = 0;
            // The Compute function MUST return a pointer just after the last element, r is the place to write all results.
            virtual float *Compute(int *IndexSerial, int LenghtOfIndex, float *r) = 0;


        private:
            const char *ModuleName;
            BioNotations *Assoc_Note;
            std::vector<PCProperty *> *ActiveProps;
            std::map<const char *, const char *, StrKMC> *ExtraParas;
            bool isRaw;
    };

    typedef PseModule *(*PseModuleInitFunction)(const char *);

    struct PseModuleInterface
    {
        const char *ModuleName;
        PseModuleInitFunction InitFunc;
    };

    struct VMExtInterface
    {
        const char *VMFunction;
        lua_CFunction CFPtr;
    };
};

extern "C"
{
    typedef UltraPse::PseModuleInterface *(*ModIFunc)();
    typedef UltraPse::VMExtInterface *(*VMIFunc)();
}

#endif // PSEMODULE_H_INCLUDED
