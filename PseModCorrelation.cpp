/****************************************************************************
 * PseModCorrelation.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                 *
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

#include "PseModCorrelation.h"
#include "PseCUI.h"

using namespace std;
using namespace UltraPse;

PseModule *UltraPse::PseModCorrelation::Init(const char *n)
{
    return new PseModCorrelation(n);
}

UltraPse::PseModCorrelation::PseModCorrelation(const char *n) :PseModule(n)
{
    Length = 0;
    Lambda = 0;
    SubType = 0;
    Omega = 0;
    CountProps = 0;
}

UltraPse::PseModCorrelation::~PseModCorrelation()
{

}

float UltraPse::PseModCorrelation::CrossCov(int *IndexSerial, int LengthOfIndex, int lag, PCProperty *p1, PCProperty *p2)
{
    float *pcValue1 = p1->GetValues();
    float *pcValue2 = p2->GetValues();
    float s = 0;
    float spc1 = 0;
    float spc2 = 0;
    for (int i = 0 ; i < LengthOfIndex - lag; i ++)
    {
        s += pcValue1[IndexSerial[i]] * pcValue2[IndexSerial[i + lag]];
        spc1 += pcValue1[IndexSerial[i]];
        spc2 += pcValue2[IndexSerial[i + lag]];
    }
    float aspc1 = spc1;
    float aspc2 = spc2;
    for (int i = 0 ; i < lag ; i ++)
    {
        aspc1 += pcValue1[IndexSerial[LengthOfIndex - lag + i]];
        aspc2 += pcValue2[IndexSerial[i]];
    }
    float avgpc1 = aspc1 / LengthOfIndex;
    float avgpc2 = aspc2 / LengthOfIndex;
    float r = (s - avgpc1 * spc2 - avgpc2 * spc1 ) / (LengthOfIndex - lag) + avgpc1 * avgpc2;
    return r;
}


/*float UltraPse::PseModCorrelation::CrossCov(int *IndexSerial, int LengthOfIndex, int lag, PCProperty *p1, PCProperty *p2)
{
    float *pcValue1 = p1->GetValues();
    float *pcValue2 = p2->GetValues();
    float s = 0;
    float spc1 = 0;
    float spc2 = 0;
    float apc1 = 0;
    float apc2 = 0;

    for (int i = 0; i < LengthOfIndex - lag ; i ++)
    {
        spc1 += pcValue1[IndexSerial[i]];
        spc2 += pcValue2[IndexSerial[i]];
    }
    apc1 = spc1 / (LengthOfIndex + 1);
    apc2 = spc2 / (LengthOfIndex + 1);

    for (int i = 0 ; i < LengthOfIndex - lag; i ++)
    {
        s += (pcValue1[IndexSerial[i]] - apc1)* (pcValue2[IndexSerial[i + lag]] - apc2);
    }

    float r = s / (LengthOfIndex - lag);
    return r;
}*/

float UltraPse::PseModCorrelation::AutoCov(int *IndexSerial, int LengthOfIndex, int lag, PCProperty *p)
{
    return CrossCov(IndexSerial, LengthOfIndex, lag, p, p);
}

float *UltraPse::PseModCorrelation::ComputeAutoCov(int *IndexSerial, int LengthOfIndex, float *r)
{
    vector<PCProperty *> *props = GetActiveProps();
    for (int i = 0; i < CountProps ; i ++)
    {
        PCProperty *p = (*props)[i];
        for (int j = 0; j < Lambda ; j ++)
        {
            float ac = AutoCov(IndexSerial, LengthOfIndex, j + 1, p);
            r[i*CountProps + j] = ac;
        }
    }
    return r += Length;
}

float *UltraPse::PseModCorrelation::ComputeCrossCov(int *IndexSerial, int LengthOfIndex, float *r)
{
    vector<PCProperty *> *props = GetActiveProps();
    int r_offset = 0;
    for (int i = 0; i < CountProps; i ++)
    {
        PCProperty *p1 = (*props)[i];
        for (int j = 0 ; j < CountProps ; j++)
        {
            PCProperty *p2 = (*props)[j];
            if (i == j)
                continue;
            for (int k = 0; k < Lambda ; k ++)
            {
                float ac = CrossCov(IndexSerial, LengthOfIndex, k + 1, p1, p2);
                r[r_offset] = ac;
                r_offset ++;
            }
        }
    }
    return r += Length;
}

float *UltraPse::PseModCorrelation::ComputeAutoCrossCov(int *IndexSerial, int LengthOfIndex, float *r)
{
    vector<PCProperty *> *props = GetActiveProps();
    int r_offset = 0;
    for (int i = 0; i < CountProps; i ++)
    {
        PCProperty *p1 = (*props)[i];
        for (int j = 0 ; j < CountProps ; j++)
        {
            PCProperty *p2 = (*props)[j];
            for (int k = 0; k < Lambda ; k ++)
            {
                float ac = CrossCov(IndexSerial, LengthOfIndex, k + 1, p1, p2);
                r[r_offset] = ac;
                r_offset ++;
            }
        }
    }
    return r += Length;
}

int UltraPse::PseModCorrelation::GetLength()
{
    return Length;
}

float *UltraPse::PseModCorrelation::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    switch(SubType)
    {
        case 1:
            return ComputeAutoCov(IndexSerial,LengthOfIndex, r);
            break;
        case 2:
            return ComputeCrossCov(IndexSerial, LengthOfIndex, r);
            break;
        case 3:
            return ComputeAutoCrossCov(IndexSerial, LengthOfIndex, r);
            break;
        default:
            break;
    };
    return r + Length;
}

void UltraPse::PseModCorrelation::Config()
{
    CountProps = GetActiveProps()->size();
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

    switch (SubType)
    {
    case 1: //AutoCovariance
        Length = CountProps * Lambda;
        break;
    case 2: //CrossCov
        Length = CountProps * (CountProps - 1) * Lambda;
        break;
    case 3: //Auto-CrossCov
        Length = CountProps * CountProps * Lambda;
        break;
    default: //Error
        break;
    };
}
