/****************************************************************************
 * Operator.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                          *
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

#include "Operator.h"
#include "RawProperty.h"

using namespace UltraPse;
using namespace std;

UltraPse::Operator::Operator()
{
    Results = new vector<PseResult *>();
    Engine = 0;
    DataSource = 0;
    count_batch = 0;
    ifHandle = 0;
    ofHandle = 0;
    Parser = 0;
    FileTrunk = 0;
    ActiveProps = 0;
    lua_vm = 0;
    PCLib = 0;
    ActiveProps = 0;
}

UltraPse::Operator::~Operator()
{
    for (PseResult *&p : *Results)
    {
        delete p;
        p = 0;
    }
    delete Results;
    Results = 0;
}

void UltraPse::Operator::CreateLuaVM()
{
    if (Task->IsTask())
    {
        lua_vm = new LuaVM();
        lua_vm->InitVM();
        lua_vm->ExecChunk(Task->GetLuaExpr());
        lua_vm->Execute(Task->GetTaskFile());
        lua_vm->ConfigVM();
    }
}

void UltraPse::Operator::SetTask(PseTask *t)
{
    Task = t;
}

void UltraPse::Operator::BatchNotify()
{
    count_batch ++;
    vector<BioSequence *> *t_list = DataSource->GetList();
	for (BioSequence *&p : *t_list)
    {
        PseResult *r = Engine->Execute(p);
        if (r)
            Results->push_back(r);
    }
	
	/*
	int cnt_Seq_bat = t_list->size();
	int i = 0;
	#pragma omp parallel for private(i) schedule(guided)
	for (i = 0 ; i < cnt_Seq_bat ; i ++)
	{
		BioSequence *p = (*t_list)[i];
		PseResult *r = Engine->Execute(p);
		if (r)
			Results->push_back(r);
	}
	*/
    ExportResults();
    ClearResultBuffer();
}

void UltraPse::Operator::ClearResultBuffer()
{
    for (PseResult *&p : *Results)
    {
        delete p;
        p = 0;
    }
    delete Results;
    Results = new vector<PseResult *>();
}

void UltraPse::Operator::ExportResults()
{
    for (PseResult *&p : *Results)
    {
        DisplayResult(ofHandle, p);
    }
}

void UltraPse::Operator::PrepareFileHandles()
{
    const char *ofn = Task->GetOutputFile();
    if (ofn)
        ofHandle = fopen(ofn, "w");
    else
        ofHandle = stdout;

    const char *ifn = Task->GetInputFile();
    if (ifn)
        ifHandle = fopen(ifn, "r");
    else
        ifHandle = stdin;

    SetupDisplayHandle(Task->GetResultFormat());
}

void UltraPse::Operator::CreatNotation()
{
    //Reach here, all notations have been defined, including those in the task file
    const char *NotationName = Task->GetNotationName();
    if (!NotationName)
        NotationName = UltraPse::DefaultNotationName;

    Note = BioNotations::GetNotationFromLib(NotationName); //If there is no such notation, just error and abort!

    //Reach here, the Note object must has been created.
    //See whether we have a matched library in our machine.
    if (PropertyLibRegistry.find(NotationName) != PropertyLibRegistry.end())
    {
        PCLib = new PCLibrary(PropertyLibRegistry[NotationName], Note);
    }

    //Props names are arbitarily defined until here. No checking before.
    //These prop names may include those defined in the task file and errors
    vector<const char *> *PropNames = Task->GetPropNames();
    vector<PCProperty *> t_props;
    for (const char *&p : *PropNames)
    {
        //See whether we can find a name in the Library?
        //If PCLib exist, try request the prop object. If zero is returned, that means the requested props do not exist in the lib.
        //Otherwise, the requested props will be added into the ActiveProperty map by the PCLib.
        PCProperty *t = PCLib ? PCLib->GetProperty(p) : 0;
        if (!t)
        {
            //The requested property does not exist in the lib or the lib does not exist at all.
            //Try create it from RawProperties
            if (RawPropLib.find(p) != RawPropLib.end())
            {
                //Request prop exist in the Raw lib, Try to create it
                const char *tempName = RawPropLib[p]->GetTemplate();
                PCProperty *PropTemplate = PCLib ? PCLib->GetProperty(tempName) : 0;
                PCProperty *NewProp = RawPropLib[p]->Convert(Note, true, PropTemplate);
                //If conversion is correct, store the property temporarily
                if (NewProp)
                    t_props.push_back(NewProp);
                else
                    ErrorAbort(ErrorCode::NoSuchProperty);
            }
            else
            {
                //The requested property does not exist in the lib and the raw props.
                //Error and Abort
                ErrorAbort(ErrorCode::NoSuchProperty);
            }
        }
    }
    //Combine the PCLib (if exist) and the temp raw properties
    ActiveProps = PCLib ? PCLib->GetActiveProperties() : new vector<PCProperty *>();
    for (PCProperty *t : t_props)
    {
        ActiveProps->push_back(t);
    }
}

void UltraPse::Operator::CreatDataParsers()
{
    DataSource = new BioSequenceList();
    DataSource->SetValidOption(Task->GetValidate());
    DataSource->SetOperator(this);
    Parser = new FastaParser(Note, DataSource);
    FileTrunk = new BlockFile(ifHandle, Parser);
}

void UltraPse::Operator::Prepare()
{
    PrepareFileHandles();
    CreatNotation();
    CreatEngine();
    CreatDataParsers();
}

void UltraPse::Operator::Execute()
{
    if (Task->IsQuery())
    {
        Query(Task->GetQueryMode());
    }
    else
    {
        if (FileTrunk)
            FileTrunk->Execute();
        if (Task->GetValidate())
            Engine->ReportErrorIDs();
    }
}

void UltraPse::Operator::Query(const char *q)
{
    if (strcasecmp(q, "mode") == 0)
    {
        PseEngine::ReportModuleLibraryStatus();
        Engine->ListModules();
    }
    else if (strcasecmp(q,"prop") == 0)
    {
        PCLib->ListProperty();
    }
    else if (strcasecmp(q,"note") == 0)
    {
        BioNotations::DisplayLibrary();
    }
    else if (strcasecmp(q,"all") == 0)
    {
        printf("[Notations]\n");
        BioNotations::DisplayLibrary();
        printf("==================\n");
        printf("[Properties]\n");
        PCLib->ListProperty();
        printf("==================\n");
        printf("[Engine]\n");
        PseEngine::ReportModuleLibraryStatus();
        Engine->ListModules();
        printf("==================\n");
    }
    else
    {
        ErrorAbort(ErrorCode::InvalideArgument);
    }
}

void UltraPse::Operator::CreatEngine()
{
    Engine = new PseEngine(Note);
    Engine->SetActiveProps(this->ActiveProps);
    Engine->SetExtraParas(Task->GetParas());

    vector<const char *> *mod_names = Task->GetModNames();
    for (const char *&p : *mod_names)
    {
        char *t_mod = strupr(strdup(p));
        Engine->AddModuleFromLibrary(t_mod);
        //TODO: Here, we can not delete the t_mod, because it is used as the name of the module until computation finished.
        //To solve memory leak, this pointer must be kept until exit.
        //delete t_mod;
        //t_mod = 0;
    }
    Engine->Prepare();
}
