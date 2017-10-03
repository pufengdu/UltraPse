/****************************************************************************
 * PseResult.h - Copyright 2017 Pu-Feng Du, Ph.D.                           *
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

#include "BioSequence.h"

#ifndef PSERESULT_H_INCLUDED
#define PSERESULT_H_INCLUDED

namespace UltraPse
{
    enum PseResultFormat {CSV, TSV, SVM};
    class PseResult
    {
        public:
            PseResult(BioSequence *p);
            ~PseResult();
            float *InitResult(int l);
            float *GetSerial();
            int GetLength();
            void DisplayCSV(FILE *fh);
            void DisplayTSV(FILE *fh);
            void DisplaySVM(FILE *fh);
        private:
            float *PseSerial;
            int Length;
            BioSequence *Assoc_Seq;
    };

    void DisplayCSV(FILE *fh, PseResult *r);
    void DisplayTSV(FILE *fh, PseResult *r);
    void DisplaySVM(FILE *fh, PseResult *r);
    extern void (*DisplayResult)(FILE *, PseResult *);
    extern void (*DisplayFuncs[])(FILE *, PseResult *);
    void SetupDisplayHandle(PseResultFormat t);
};

#endif // PSERESULT_H_INCLUDED
