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

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <string>
#include <ostream>
using namespace std;

class NxtEx{
        string str;
    public:
        NxtEx();
        NxtEx(string str);
        string toString();
        ~NxtEx(){};
};
class NxtNoBluetoothAdapterFoundEx:public NxtEx{
    public:
        NxtNoBluetoothAdapterFoundEx():NxtEx("No bluetooth adapter found on local machine"){}
};
class NxtNotPresentEx:public NxtEx{
    public:
        NxtNotPresentEx():NxtEx("NXT not found on USB bus"){}
};
class NxtConfigurationErrorEx:public NxtEx{
    public:
        NxtConfigurationErrorEx():NxtEx("Error trying to configure the NXT USB link"){}
};
class NxtInUseEx:public NxtEx{
    public:
        NxtInUseEx():NxtEx("NXT USB interface is already claimed by another program"){}
};
class NxtWriteErrorEx:public NxtEx{
    public:
        NxtWriteErrorEx():NxtEx("Bus write error"){}
};
class NxtReadErrorEx:public NxtEx{
    public:
        NxtReadErrorEx():NxtEx("Bus read error"){}
};
class NxtHandshakeFailedEx:public NxtEx{
    public:
        NxtHandshakeFailedEx():NxtEx("NXT handshake failed"){}
};
class NxtFileErrorEx:public NxtEx{
    public:
        NxtFileErrorEx():NxtEx("File open/handling error"){}
};
class NxtInvalidFirmwareEx:public NxtEx{
    public:
        NxtInvalidFirmwareEx():NxtEx("Invalid firmware image"){}
};

#endif /* __EXCEPTIONS_H__ */
