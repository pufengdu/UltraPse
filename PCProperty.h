/****************************************************************************
 * PCProperty.h - Copyright 2017 Pu-Feng Du, Ph.D.                          *
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

#ifndef PCPROPERTY_H_INCLUDED
#define PCPROPERTY_H_INCLUDED

namespace UltraPse
{
    class PCProperty
    {
        public:
            PCProperty(BioNotations *a);
            ~PCProperty();
            void SetValue(char *Note, float v);
            const char *GetID();
            void SetID(const char *d);
            const char *GetComment();
            void SetComment(const char *s);
            void CopyValues(float *s);
            float *GetValues(){return Values;}
            BioNotations *GetNote() {return Assoc_Note;}
            void PreCompute();
            float **GetDiffMatrix();
            float **GetDotMatrix();
            PCProperty *SelfClone();
        private:
            const char *ID;
            float *Values;
            const char *Comment;
            float **DiffMatrix;
            float **DotMatrix;
            int cnt_notes;
            bool isActive;
            BioNotations *Assoc_Note;
    };
}
#endif // PCPROPERTY_H_INCLUDED
