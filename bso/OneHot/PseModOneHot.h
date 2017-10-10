#include "../../PseModule.h"
#include "../Template/PseModInterface.h"
#include <lua.hpp>

#ifndef PSEMODONEHOT_H_INCLUDED
#define PSEMODONEHOT_H_INCLUDED


namespace UltraPse
{
    //User defined engine module
    class PseModOneHot : public PseModule
    {
        public:
            PseModOneHot(const char *n);
            ~PseModOneHot();
            void Config();
            int GetLength();
            float *Compute(int *IndexSerial, int LengthOfIndex, float *r);
            static PseModule *Init(const char *n);
        private:
            int Length;
            int Count_Notes;
    };
};



#endif // PSEMODONEHOT_H_INCLUDED
