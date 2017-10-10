#include "PseModOneHot.h"
#include "../Template/PseModInterface.h"

using namespace std;
using namespace UltraPse;

//Module Registry
PseModuleInterface UltraPse::ModuleInterfaces[] =
{
    {"ONEHOT", UltraPse::PseModOneHot::Init},
    {0,0}
};

VMExtInterface UltraPse::VMInterfaces[] =
{
    {0,0}
};

PseModule * UltraPse::PseModOneHot::Init(const char *n)
{
    return new PseModOneHot(n);
}

UltraPse::PseModOneHot::PseModOneHot(const char *n) : PseModule (n)
{
    Length = 0;
    Count_Notes = 0;
}

UltraPse::PseModOneHot::~PseModOneHot()
{
    Length = 0;
}

void UltraPse::PseModOneHot::Config()
{
    BioNotations *b = GetNotations();
    Count_Notes = b->GetTotalNotations();
    std::map<const char*, const char*, StrKMC> *paras = GetExtraParas();
    if (paras->find("_cmd_lambda") != paras->end())
    {
        Length = Count_Notes * strtol((*paras)["_cmd_lambda"], 0, 0);
    }
    else
    {
        fprintf(stderr, "[ONE-HOT] You must specify -l option for your sequence length.\n");
        exit(0);
    }
}

int UltraPse::PseModOneHot::GetLength()
{
    return Length;
}

float *UltraPse::PseModOneHot::Compute(int *IndexSerial, int LengthOfIndex, float *r)
{
    for (int i = 0 ; i < LengthOfIndex ; i ++)
    {
        r[i * Count_Notes + IndexSerial[i]] = 1;
    }
    return r + Length;
}

