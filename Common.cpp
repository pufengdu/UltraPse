/****************************************************************************
 * Common.cpp - Copyright 2017 Pu-Feng Du, Ph.D.                            *
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

#include "Common.h"
#include "PseModule.h"
#include "PseEngine.h"
#include "LuaVM.h"

using namespace UltraPse;
using namespace std;

char UltraPse::FastaCommentSep = '|';
vector<void *> UltraPse::_MemoryRepo;
map<const char *, void *, StrKMC> UltraPse::_HandleRepo;

void UltraPse::ErrorAbort(ErrorCode e)
{
    exit((int)e);
}

off_t UltraPse::FileSize(const char *fn)
{
    struct stat b;
    if(!stat(fn,&b))
        return b.st_size;

    else
        return -1;
}

int UltraPse::filelength(int fd)
{
	struct stat b;
	if (!fstat(fd, &b))
		return (int)b.st_size;
	else
		return -1;
}

char *UltraPse::strupr(char *s)
{
	char *p = s;
	while (*p)
	{
		*p = toupper(*p);
		p++;
	}
	return s;
}

char *UltraPse::Trim(char *s)
{
    if (!*s) return s; //Very important!!!
    char *h = s, *t = s, *p = s;
    while (*h == ' ')
        h ++;
    while (*p)
    {
        if (*p != ' ' && *p != '\r')
            t = p;
        p ++;
    }

    *(t + 1) = 0;
    return h;
}

bool UltraPse::CheckPrefix(char *s, const char *prefix)
{
    char *p = s;
    const char *t = prefix;
    bool r = true;
    while (*p && *t)
    {
        r = r && (*p == *t);
        p ++;
        t ++;
    }
    return r;
}

char **UltraPse::LoadTextLines(const char *fn)
{
    off_t fs = FileSize(fn);
    if (fs == -1)
        return 0;
    FILE *fh = fopen(fn,"rb");
    if (!fh)
        return 0;
    char *r = new char[fs]{};
    fread(r,sizeof(char),fs,fh);
    fclose(fh);

    int cnt_lines = 0;
    for (int i = 0 ; i < fs ; i ++)
    {
        if (r[i] == '\n')
        {
            cnt_lines ++;
            r[i] = 0;
        }
    }
    char **lines = new char*[cnt_lines + 1];
    for (int i = 0 ; i < cnt_lines + 1 ; i ++)
        lines[i] = 0;
    lines[0] = r;
    int l_idx = 0;
    for (int i = 0 ; i < fs ; i ++)
    {
        if (r[i] == 0)
        {
            l_idx ++;
            lines[l_idx] = r + i + 1;
        }
    }
    lines[cnt_lines] = 0;
    return lines;
}

char *UltraPse::BlockToCode(Byte *b, char *CodePrefix)
{
    int *pcnt_property = (int*)b;
    int *pcnt_byte = pcnt_property + 1;
    int cnt_byte = *pcnt_byte;
    ostringstream oss;
    oss << CodePrefix << " = {0x" << hex << (int)b[0];
    for (int i = 1 ; i < cnt_byte ; i ++)
    {
        oss << ", 0x" << hex << (int)b[i];
    }
    oss << "};";
    return strdup(oss.str().c_str());
}

bool UltraPse::StrKeyMapComp(const char *a, const char *b)
{
    return strcmp(a,b) < 0;
}

float *UltraPse::NormalizeValues(float *data, int n)
{
    float m = 0;
    float v = 0;
    float v1 = 0;
    float s = 0;
    float sq = 0;
    for (int i = 0; i < n; i ++)
    {
        s += data[i];
        sq += data[i] * data[i];
    }
    m = s / n;
    // This is very important, as PseKNC / Pse-in-One uses this kind of stddev
    v1 = (sq / n - m * m) * n / (n - 1);
    v = sqrt(v1);
    for (int i = 0 ; i < n; i ++)
        data[i] = (data[i] - m) / v;
    return data;
}

int UltraPse::SplitCSV(char *s, char ***out_fields)
{
    int cf = 2;
    for (char *p = s; *p ; p ++)
        cf += (*p == ',');
    *out_fields = new char*[cf];
    memset(*out_fields, 0, sizeof(char*) * cf);
    (*out_fields)[0] = s;
    int idx = 1;
    for (char *p = s; *p ; p ++)
    {
        if (*p == ',')
        {
            *p = 0;
            p ++;
            (*out_fields)[idx] = p;
            idx ++;
        }
    }
    return cf;
}


