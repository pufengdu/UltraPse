/****************************************************************************
 * PseEngine.h - Copyright 2017 Pu-Feng Du, Ph.D.                           *
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
#include "PseModule.h"
#include "PseModuleRaw.h"
#include "PseResult.h"
#include "PCProperty.h"
#include "Common.h"

#ifndef PSEENGINE_H_INCLUDED
#define PSEENGINE_H_INCLUDED

namespace UltraPse
{
    extern std::map<const char *, PseModuleInitFunction, StrKMC> EngineModuleLibrary;

    class PseEngine
    {
        public:
            PseEngine(BioNotations *b);
            ~PseEngine();
            void AddModule(PseModule *p);
            void Prepare();
            PseResult *Execute(BioSequence *s);
            void Cleanup();
            void ListModules();
            void AddModuleFromLibrary(const char *n);
            void ReportErrorIDs();
            void SetActiveProps(std::vector<PCProperty *> *ps)
            {
                ActiveProps = ps;
            }
            void SetExtraParas(std::map<const char*, const char*, StrKMC> *ep)
            {
                ExtraParas = ep;
            }

            const char *GetExtraPara(const char *n)
            {
                if(ExtraParas)
                {
                    if (ExtraParas->find(n) != ExtraParas->end())
                    {
                        return (*ExtraParas)[n];
                    }
                    else
                    {
                        return 0;
                    }
                }
                return 0;
            }

            PCProperty *GetPropObject(const char *n)
            {
                if (ActiveProps)
                {
                    for (PCProperty *&p : *ActiveProps)
                    {
                        if (strcasecmp(p->GetID(), n) == 0)
                        {
                            return p;
                        }
                    }
                    return 0;
                }
                return 0;
            }

            static void RegisterModuleToLibrary(const char *n, PseModuleInitFunction f);
            static void ReportModuleLibraryStatus();
            static PseEngine *EngineVMPtr;

        private:
            std::vector<PseModule *> Modules;
            std::vector<PseModuleRaw *> RawModules;
            std::vector<const char *> ErrorSeqIDs;
            std::vector<PCProperty *> *ActiveProps;
            std::map<const char *, const char *, StrKMC> *ExtraParas;
            BioNotations *Assoc_Note;
            int ResultLength;
    };
}

#endif // PSEENGINE_H_INCLUDED
