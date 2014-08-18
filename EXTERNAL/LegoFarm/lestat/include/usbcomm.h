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

#ifndef __USBCOMM_H__
#define __USBCOMM_H__

#include <usb.h>
#include "exceptions.h"
#include "comms.h"

class USBComm:public Comms{
    private:
        static int VENDOR_LEGO;
//        static int VENDOR_ATMEL;
        static int PRODUCT_NXT;
//        static int PRODUCT_SAMBA;


        struct usb_device *m_dev;
        struct usb_dev_handle *m_hdl;
//        bool is_in_reset_mode;

    public:
        USBComm();
        ~USBComm();
        void find();
        void connect(const char *mac);
        void disconnect();
//        bool nxt_in_reset_mode();
        void sendBuffer(const char *buf, unsigned short int len);
        void sendString(const string str);
        void recvBuffer(char *buf, unsigned short int len);



};
#endif /* __USBCOMM_H__ */
