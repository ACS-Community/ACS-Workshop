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

#include "message.h"
Message::Message(char type, char opcode){
    msg.push_back(type);
    msg.push_back(opcode);
}
void Message::reset(char type, char opcode){
    msg.clear();
    msg.push_back(type);
    msg.push_back(opcode);
}

void Message::addS8(signed char a){
    msg.push_back(a);
}

void Message::addU8(unsigned char a){
    msg.push_back(a);
}

void Message::addS16(signed short int a){
#ifdef _NXT_LITTLE_ENDIAN
    msg.push_back(a&0xff);
    msg.push_back((a>>8)&0xff);
#elif _NXT_BIG_ENDIAN
    msg.push_back((a>>8)&0xff);
    msg.push_back(a&0xff);
#endif

}

//#include <iostream>
void Message::addU16(unsigned short int a){
#ifdef _NXT_LITTLE_ENDIAN
    msg.push_back(a&0xff);
    msg.push_back((a>>8)&0xff);
    //std::cout<<(a&0xff)<<"   "<<(a>>8)<<std::endl;
#elif _NXT_BIG_ENDIAN
    msg.push_back((a>>8)&0xff);
    msg.push_back(a&0xff);
#endif

}


void Message::addS32(signed int a){
#ifdef _NXT_LITTLE_ENDIAN
    msg.push_back(a&0xff);
    msg.push_back((a>>8)&0xff);
    msg.push_back((a>>16)&0xff);
    msg.push_back((a>>24)&0xff);
    //std::cout<<(a&0xff)<<"   "<<(a>>8)<<std::endl;
#elif _NXT_BIG_ENDIAN
    msg.push_back((a>>24)&0xff);
    msg.push_back((a>>16)&0xff);
    msg.push_back((a>>8)&0xff);
    msg.push_back(a&0xff);
#endif
    
}

void Message::addU32(unsigned int a){
#ifdef _NXT_LITTLE_ENDIAN
    msg.push_back(a&0xff);
    msg.push_back((a>>8)&0xff);
    msg.push_back((a>>16)&0xff);
    msg.push_back((a>>24)&0xff);
    //std::cout<<(a&0xff)<<"   "<<(a>>8)<<std::endl;
#elif _NXT_BIG_ENDIAN
    msg.push_back((a>>24)&0xff);
    msg.push_back((a>>16)&0xff);
    msg.push_back((a>>8)&0xff);
    msg.push_back(a&0xff);
#endif
    
}

char *Message::getMsg(){
    //return msg.toArray();
    char *retn=new char[msg.size()];
    std::copy(msg.begin(), msg.end(), retn);
    //for(unsigned int i=0;i<msg.size();i++){
    //    retn[i]=msg[i];
    //}
    return retn;
}

unsigned int Message::getLen(){
    return msg.size();
}
signed short int 
Message::bytes2S16(char *buf){
    signed short int ret=0;
    ret = (0xff & buf[0]) | ((0xff & buf[1])<<8);
    return ret;
}
unsigned short int 
Message::bytes2U16(char *buf){
    unsigned short int ret=0;
    ret += (0xff & buf[0]) | ((0xff & buf[1])<<8);
    return ret;
}
signed int 
Message::bytes2S32(char *buf){
    signed int ret=0;
    ret = (0xff & buf[0]) | ((0xff & buf[1])<<8) | ((0xff & buf[2])<<16) | ((0xff & buf[3])<<24);
    return ret;
}
unsigned int 
Message::bytes2U32(char *buf){
    signed int ret=0;
    ret =   (0xff & buf[0])
         | ((0xff & buf[1])<<8)
         | ((0xff & buf[2])<<16)
         | ((0xff & buf[3])<<24);
    return ret;
}


