/****************************************************************************
 * PseEngine.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                         *
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

#include "PseEngine.h"
#include "BioSequence.h"
#include "PseResult.h"
#include "BioNotations.h"
#include "PseModComposition.h"
#include "PseModDipeptide.h"
#include "PseModPseAAC.h"
#include "PseModCorrelation.h"
#include "PseModLua.h"
#include "PseModPseb3.h"

using namespace std;
using namespace UltraPse;

map<const char *, PseModuleInitFunction, StrKMC> UltraPse::EngineModuleLibrary
(
    {
        {"COMP", PseModComposition::Init},
        {"DPC", PseModDipeptide::Init},
        {"PSE", PseModPseAAC::Init},
        {"COV", PseModCorrelation::Init},
        {"LUA", PseModLua::Init},
        {"PSEB3", PseModPseb3::Init}
    },
    UltraPse::StrKeyMapComp
);

UltraPse::PseEngine *UltraPse::PseEngine::EngineVMPtr = 0;

UltraPse::PseEngine::PseEngine(BioNotations *b)
{
    Assoc_Note = b;
    ResultLength = 0;
    ActiveProps = 0;
    ExtraParas = 0;
    EngineVMPtr = this;
}

UltraPse::PseEngine::~PseEngine()
{
    this->Cleanup();
}

void UltraPse::PseEngine::AddModule(UltraPse::PseModule *p)
{
    p->SetActiveProps(ActiveProps);
    p->SetExtraParas(ExtraParas);
    if (p->GetRaw())
        RawModules.push_back(reinterpret_cast<PseModuleRaw *>(p));
    else
        Modules.push_back(p);
}

void UltraPse::PseEngine::AddModuleFromLibrary(const char *n)
{
    if (EngineModuleLibrary.find(n) == EngineModuleLibrary.end())
    {
        ErrorAbort(ErrorCode::UndefinedEngineModule);
    }
    PseModuleInitFunction f = EngineModuleLibrary[n];
    AddModule(f(n));
}

void UltraPse::PseEngine::RegisterModuleToLibrary(const char *n, UltraPse::PseModuleInitFunction f)
{
    EngineModuleLibrary[n] = f;
}

void UltraPse::PseEngine::ReportModuleLibraryStatus()
{
    int n = EngineModuleLibrary.size();
    printf("Module library can provide %d type of engine module\n", n);
    int k = 0;
    printf("#\tNAME\n");
    printf("-------\n");
    for (auto &p : EngineModuleLibrary)
    {
        printf("%d.\t%s\n", k, p.first);
        k ++;
    }
}

void UltraPse::PseEngine::Prepare()
{
    int TotalLength = 0;
    for (PseModule *p : Modules)
    {
        p->SetNotations(Assoc_Note);
        p->Config();
        TotalLength += p->GetLength();
    }

    for (PseModuleRaw *p : RawModules)
    {
        p->SetNotations(Assoc_Note);
        p->Config();
        TotalLength += p->GetLength();
    }
    ResultLength = TotalLength;
}

UltraPse::PseResult *UltraPse::PseEngine::Execute(BioSequence *s)
{
    if (!s->IsValid())
    {
        ErrorSeqIDs.push_back(strdup(s->GetID()));
        return 0;
    }
    int t_l = s->GetNumberOfIndex();
    int *t_p = s->GetIndexSerial();
    PseResult *r = new PseResult(s);
    float *t_fill = r->InitResult(ResultLength);
    for (PseModule *p : Modules)
    {
        t_fill = p->Compute(t_p, t_l, t_fill);
    }

    for (PseModuleRaw *p : RawModules)
    {
        p->SetSeqID(s->GetID());
        char *seq = s->GetSequenceClone();
        p->SetSeqClone(seq);
        t_fill = p->Compute(t_p, t_l, t_fill);
        //delete [] seq;
    }
    return r;
}

void UltraPse::PseEngine::ListModules()
{
    int n = Modules.size();
    printf("Current execution engine are composed by %d modules:\n", n);
    for (int i = 0 ; i < n ; i ++)
    {
        printf("%d. %s\n", i + 1, Modules[i]->GetName());
    }
    printf("Total descriptors : %d\n", ResultLength);
}

void UltraPse::PseEngine::Cleanup()
{
    while (!Modules.empty())
    {
        PseModule *p = Modules.back();
        Modules.pop_back();
        delete p;
        p = 0;
    }

    for (const char *&p : ErrorSeqIDs)
    {
        delete p;
        p = 0;
    }
    ErrorSeqIDs.clear();
}

void UltraPse::PseEngine::ReportErrorIDs()
{
    for (const char *&p : ErrorSeqIDs)
    {
        fprintf (stderr, "%s : ERROR! Not conforming required notation definitions\n", p);
    }
    fprintf(stderr, "%d sequences contain errors\n", static_cast<int>(ErrorSeqIDs.size()));
}
