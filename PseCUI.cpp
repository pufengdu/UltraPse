/****************************************************************************
 * PseCUI.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                            *
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

#include "PseCUI.h"
#include "PseTask.h"

const char *argp_program_version = "UltraPse v1.0";
const char *argp_program_bug_address = "Dr. Pu-Feng Du: <PufengDu@gmail.com>";

const char *_cmd_lambda_name = "_cmd_lambda";
const char *_cmd_omega_name = "_cmd_omega";
const char *_cmd_subtype = "_cmd_subtype";
const char *_cmd_ktuple = "_cmd_ktuple";

namespace UltraPse
{

    char doc[] = "UltraPse -- An ultra-fast extensible software platform for biological sequence representations.";
    char args_doc[] = "";

    /* The options we understand. */
    struct argp_option options[] =
    {
        {"in", 'i',    "FILE",  0,  "A FASTA format file for input. The comment line of a sequence in this file should be unique. If you do not specify an in-file, the program will try to read it from keyboard."},
        {"out",   'o',    "FILE",  0,  "A file for storing the results. If you do not specify an out-file, the results will be written on screen." },
        {"note",'n',"{stdprot|didna|dirna|tridna}", 0, "Sequence definition types. There are four internal types, if the user want to use their own sequence type, they have to use a task definition file."},
        {"format",  'f',    "{svm|tsv|csv}",  0,"Output format."},
        {"mode",  'm',    "{comp|pse|dpc|cov|lua|pseb3|user,LIB_OBJECT,MOD_NAME}", 0,  "Representation module choices. This option can appear for multiple times."},
        {"user-task", 'u', "TASK-FILE", 0, "Task description file."},
        {"query", 'q', "{prop|mode|note|all}",0,"Query status of everything."},
        {"property", 'p', "PROP-ID", 0, "Activate the property PROP-ID for current task. This option can appear for multiple times."},
        {"omega", 'w', "W", 0, "A decimal number for pseknc and pseaac mode, parameter w. This option can have user defined purpose."},
        {"lambda", 'l', "L", 0, "An integer for maximal lag. This option can have user defined purpose."},
        {"type",'t',"T",0, "Type parameter in pseknc, pseaac. This option can have user defined purpose."},
        {"k-tuple",'k',"K",0,"Use K-tuples in computation. This option can have user defined purpose."},
        {"arguments",'a',"LUA-EXPR",0,"Lua expression to provide extra module-specific parameters. This option can appear for multiple times."},
        {"validate",'v',0,0,"Validate sequences automatically. Sequence with invalid notations will be dropped automatically."},
        { 0 }
    };

    error_t parse_opt (int key, char *arg, struct argp_state *state)
    {
      /* Get the input argument from argp_parse, which we
         know is a pointer to our arguments structure. */
        PseTask *CurrentTask = (PseTask*)(state->input);
        switch (key)
        {
            case 'i':
                CurrentTask->SetInputFile(arg);
                break;
            case 'o':
                CurrentTask->SetOutputFile(arg);
                break;
            case 'f':
                CurrentTask->SetOutputFormat(arg);
                break;
            case 'n':
                CurrentTask->SetNotationName(arg);
                break;
            case 'm':
                CurrentTask->AddModule(arg);
                break;
            case 'u':
                CurrentTask->SetTaskFile(arg);
                break;
            case 'q':
                CurrentTask->ActivateQueryMode(arg);
                break;
            case 'p':
                CurrentTask->AddProperty(arg);
                break;
            case 'w':
                CurrentTask->AddExtraPara(_cmd_omega_name, arg);
                break;
            case 'l':
                CurrentTask->AddExtraPara(_cmd_lambda_name, arg);
                break;
            case 't':
                CurrentTask->AddExtraPara(_cmd_subtype, arg);
                break;
            case 'k':
                CurrentTask->AddExtraPara(_cmd_ktuple, arg);
                break;
            case 'a':
                CurrentTask->SetLuaExpr(arg);
                break;
            case 'v':
                CurrentTask->SetValidate(true);
                break;
            default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }

    struct argp argp = { options, parse_opt, args_doc, doc };

    PseTask *ParseCUI(int argc, char *argv[])
    {
        PseTask *r = new PseTask();
        argp_parse(&argp, argc, argv, 0, 0, r);
        return r;
    }

};
