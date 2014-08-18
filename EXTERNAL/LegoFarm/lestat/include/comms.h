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

#ifndef __COMMS_H__
#define __COMMS_H__

#include "exceptions.h"

class Comms{
    private:
    public:
        Comms();
        virtual ~Comms();
        virtual void find()=0;//connects to any device found
        virtual void connect(const char *mac)=0;//connect to specified device
        virtual void disconnect()=0;//disconnect from device
        //bool nxt_in_reset_mode();
        virtual void sendBuffer(const char *buf, unsigned short int len)=0;//send raw data through socket
        virtual void sendString(const string str)=0;//send string through socket
        virtual void recvBuffer(char *buf, unsigned short int len)=0;//receive data



};
#endif /* __COMMS_H__ */
