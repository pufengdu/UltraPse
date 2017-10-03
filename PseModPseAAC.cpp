/****************************************************************************
 * PseModPseAAC.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                      *
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

#include "PseModPseAAC.h"
#include "PseCUI.h"
#include "PCLib.h"

using namespace std;
using namespace UltraPse;

PseModule *UltraPse::PseModPseAAC::Init(const char *n)
{
    return new PseModPseAAC(n);
}

UltraPse::PseModPseAAC::PseModPseAAC(const char *n) : PseModule(n)
{
    Length = 0;
    LengthAAC = 0;
    CountProps = 0;
    ActiveMatrix = 0;
}

UltraPse::PseModPseAAC::~PseModPseAAC()
{
    delete [] ActiveMatrix;
    ActiveMatrix = 0;
}

int UltraPse::PseModPseAAC::GetLength()
{
    return Length;
}

float *UltraPse::PseModPseAAC::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    float *p = ComputeAAC(IndexSerial, LengthOfIndex, r);
    if (SubType == 1)
        ComputeType1(IndexSerial, LengthOfIndex, p);
    if (SubType == 2)
        ComputeType2(IndexSerial, LengthOfIndex, p);

    //Final adjustment using omega and output factor
    float s = 0;
    for (int i = 0 ; i < Length ; i ++)
        s += r[i];
    for (int i = 0 ; i < Length ; i ++)
    {
        r[i] = r[i] * 100 / s;
    }
    return r + Length;
}

void UltraPse::PseModPseAAC::Config()
{
    //TODO : Validate whether the engine notation is compatible with the property notations
    BioNotations *b = GetNotations();
    LengthAAC = b->GetTotalNotations();

    map<const char *, const char *, StrKMC> *t_paras = GetExtraParas();
    if (t_paras->find(_cmd_lambda_name) != t_paras->end())
    {
        Lambda = strtol((*t_paras)[_cmd_lambda_name], 0, 0);
    }
    if (t_paras->find(_cmd_omega_name) != t_paras->end())
    {
        Omega = strtof((*t_paras)[_cmd_omega_name], 0);
    }
    if (t_paras->find(_cmd_subtype) != t_paras->end())
    {
        SubType = strtol((*t_paras)[_cmd_subtype], 0, 0);
    }

    Length = LengthAAC;

    vector<PCProperty *> *t_props = GetActiveProps();
    CountProps = t_props->size();
    ActiveMatrix = new float**[CountProps];

    for (int i = 0 ; i < CountProps ; i ++)
    {
        PCProperty *p = (*t_props)[i];
        if (SubType == 1)
        {
            ActiveMatrix[i] = p->GetDiffMatrix();
        }
        else if (SubType == 2)
        {
            ActiveMatrix[i] = p->GetDotMatrix();
        }
        else
            ErrorAbort(ErrorCode::InvalideArgument);
    }
    if (SubType == 1)
        Length += Lambda;
    else if (SubType == 2)
        Length += Lambda * CountProps;
}

float *UltraPse::PseModPseAAC::ComputeType1(int *IndexSerial, int LengthOfIndex, float *r)
{
    for (int i = 1 ; i <= Lambda ; i ++)
    {
        float t = 0;
        for (int j = 0 ; j < LengthOfIndex - i; j ++)
        {
            for (int k = 0 ; k < CountProps ; k ++)
            {
                float f = ActiveMatrix[k][IndexSerial[j]][IndexSerial[j+i]];
                t += f;
            }
        }
        t /= CountProps;
        t /= (LengthOfIndex - i);
        r[i - 1] = t * Omega;
    }
    return r + Lambda;
}

float *UltraPse::PseModPseAAC::ComputeType2(int *IndexSerial, int LengthOfIndex, float *r)
{
    for (int i = 1 ; i <= Lambda ; i ++)
    {
        for (int k = 0 ; k < CountProps ; k ++)
        {
            float t = 0;
            for (int j = 0 ; j < LengthOfIndex - i ; j ++)
            {
                float f = ActiveMatrix[k][IndexSerial[j]][IndexSerial[j+i]];
                t += f;
            }
            t /= (LengthOfIndex - i);
            r[(i - 1)*CountProps + k] = t * Omega;
        }
    }
    return r + Lambda * CountProps;
}

float *UltraPse::PseModPseAAC::ComputeAAC(int *IndexSerial, int LengthOfIndex, float *r)
{
    for (int i = 0 ; i < LengthOfIndex ; i ++)
    {
        r[IndexSerial[i]] ++;
    }
    for (int i = 0 ; i < Length ; i ++)
    {
        r[i] /= LengthOfIndex;
        //r[i] *= 100;
    }
    return r + LengthAAC;
}
