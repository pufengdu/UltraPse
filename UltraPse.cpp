/****************************************************************************
 * UltraPse.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                          *
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

#include "UltraPse.h"

using namespace std;
using namespace UltraPse;

int main(int argc, char *argv[])
{
    Operator *App = new Operator();
    PseTask *UltraPseTask = ParseCUI(argc, argv);
    App->SetTask(UltraPseTask);
    App->CreateLuaVM();
    App->Prepare();
    App->Execute();
    delete App;
    delete UltraPseTask;
    return 0;
}
