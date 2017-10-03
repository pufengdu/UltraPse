/****************************************************************************
 * PseCUI.h - Copyright 2017 Pu-Feng Du, Ph.D.                              *
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

/**
    argp.h is not officially supported by MinGW64 platform.
    Therefore, the MSYS2 version of libargp was compiled using MinGW64.
    This program MUST be compiled by MinGW64 and linked with that version of
    libargp.a
**/

#include <argp.h>
#include "PseTask.h"

#ifndef PSECUI_H_INCLUDED
#define PSECUI_H_INCLUDED


extern const char *_cmd_lambda_name;
extern const char *_cmd_omega_name;
extern const char *_cmd_subtype;
extern const char *_cmd_ktuple;


namespace UltraPse
{
    extern char doc[];
    extern char args_doc[];
    extern struct argp_option options[];

    error_t parse_opt (int key, char *arg, struct argp_state *state);
    extern struct argp argp;
    PseTask *ParseCUI(int argc, char *argv[]);
}

#endif // PSECUI_H_INCLUDED
