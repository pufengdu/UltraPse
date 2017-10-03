/****************************************************************************
 * RawProperty.h - Copyright 2017 Pu-Feng Du, Ph.D.                         *
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

#include "PCProperty.h"
#include "BioNotations.h"
#include "Common.h"

#ifndef RAWPROPERTY_H_INCLUDED
#define RAWPROPERTY_H_INCLUDED

namespace UltraPse
{
    class RawProperty
    {
        public:
            RawProperty() : RawValues(StrKeyMapComp) {ID = 0; Comment = 0; TemplateID = 0;}
            ~RawProperty();
            void SetID(const char *id) {ID = id;}
            const char *GetID() {return ID;}
            void SetComment(const char *c) {Comment = c;}
            const char *GetComment() {return Comment;}
            void SetTemplate(const char *t) {TemplateID = t;}
            const char *GetTemplate() {return TemplateID;}
            bool AddValue(const char *n, float v)
            {
                if (RawValues.find(n) == RawValues.end())
                {
                    RawValues[n] = v;
                    return true;
                }
                else
                    return false;
            }
            PCProperty *Convert(BioNotations *b, bool AutoFill, PCProperty *TemplateProp);
            bool ValidateRawValues(BioNotations *b);
        private:
            const char *ID;
            const char *Comment;
            const char *TemplateID;
            std::map<const char*, float, StrKMC> RawValues;
    };

    extern std::map<const char*, RawProperty *, StrKMC> RawPropLib;
};

#endif // RAWPROPERTY_H_INCLUDED
