/****************************************************************************
 * PCLib.h - Copyright 2017 Pu-Feng Du, Ph.D.                               *
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
#include "PCProperty.h"

#ifndef PCLIB_H_INCLUDED
#define PCLIB_H_INCLUDED

namespace UltraPse
{
    extern std::map<const char *, unsigned char *, StrKMC> PropertyLibRegistry;

    struct PropertyIndex
    {
        int id_offset;
        int value_offset;
        int comment_offset;
    };

    class PCLibrary
    {
        public:
            PCLibrary(Byte *data_block, BioNotations *b);
            ~PCLibrary();
            PCProperty *GetProperty(const char *id);
            void AddUserProperty(PCProperty *s);
            std::vector<PCProperty *> *GetActiveProperties();
            void ListProperty();
        private:
            std::map<const char *, PropertyIndex *, StrKMC> *PCIndex;
            std::vector<PCProperty *> *ActiveProperties;
            Byte *h;
            Byte *IndexBlock;
            Byte *IDBlock;
            Byte *ValueBlock;
            Byte *CommentBlock;
            BioNotations *Assoc_Note;
            int count_props;
            int count_bytes;
    };
};

#endif // STDPROT_H_INCLUDED
