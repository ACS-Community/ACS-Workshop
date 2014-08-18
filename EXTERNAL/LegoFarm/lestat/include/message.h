/**
 *
 * Copyright 2008 Nicolas A. Barriga <nbarriga@inf.utfsm.cl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__
using namespace std;
#include <vector>

class Message{
        vector<unsigned char> msg;
    public:
        Message(char type, char opcode);
        void reset(char type, char opcode);
        void addS8(signed char a);
        void addU8(unsigned char a);
        void addS16(signed short int a);
        void addU16(unsigned short int a);
        void addS32(signed int a);
        void addU32(unsigned int a);
        char *getMsg();
        unsigned int getLen();
        static signed short int bytes2S16(char *buf);
        static unsigned short int bytes2U16(char *buf);
        static signed int bytes2S32(char *buf);
        static unsigned int bytes2U32(char *buf);
};


#endif /* __MESSAGE_H__ */
