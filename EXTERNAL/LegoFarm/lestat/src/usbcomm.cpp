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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <usb.h>
#include "usbcomm.h"
int USBComm::VENDOR_LEGO   = 0x0694;
//int Comms::VENDOR_ATMEL  = 0x03EB;
int USBComm::PRODUCT_NXT   = 0x0002;
//int Comms::PRODUCT_SAMBA = 0x6124;



USBComm::USBComm()
{
    usb_init();
    //is_in_reset_mode = false;
}
USBComm::~USBComm(){

}

void 
USBComm::connect(char const*){//TODO: implement method to access a specific usb device

}

    void 
USBComm::find()
{
    struct usb_bus *busses, *bus;

    usb_find_busses();
    usb_find_devices();

    busses = usb_get_busses();

    for (bus = busses; bus != NULL; bus = bus->next)
    {
        struct usb_device *dev;

        for (dev = bus->devices; dev != NULL; dev = dev->next)
        {
            /*if (dev->descriptor.idVendor == VENDOR_ATMEL &&
                    dev->descriptor.idProduct == PRODUCT_SAMBA)
            {
                m_dev = dev;
                is_in_reset_mode = true;
                return;
            }
            else*/ 
            if (dev->descriptor.idVendor == VENDOR_LEGO &&
                    dev->descriptor.idProduct == PRODUCT_NXT)
            {
                m_dev = dev;
                int ret;

                m_hdl = usb_open(m_dev);

                ret = usb_set_configuration(m_hdl, m_dev->config->bConfigurationValue);
                if (ret < 0)
                {
                    usb_close(m_hdl);
                    throw NxtConfigurationErrorEx();
                }

                ret = usb_claim_interface(m_hdl, m_dev->config->interface->altsetting->bInterfaceNumber);
                if (ret < 0)
                {
                    usb_close(m_hdl);
                    throw NxtInUseEx();
                }
#if 0
                // NXT handshake 
                char buf[2];
                sendString("N#");
                recvBuffer(buf, 2);
                if (memcmp(buf, "\n\r", 2) != 0)
                {
                    usb_release_interface(m_hdl, 1);
                    usb_close(m_hdl);
                    throw NxtHandshakeFailedEx();
                }
#endif
                return;
            }
        }
    }

    throw NxtNotPresentEx();
}


    void
USBComm::disconnect()
{
    int stat;
    stat=usb_release_interface(m_hdl, m_dev->config->interface->altsetting->bInterfaceNumber);
    if(stat<0)perror(NULL);
    stat=usb_close(m_hdl);
    if(stat<0)perror(NULL);
}

/*
    bool
USBComms::nxt_in_reset_mode()
{
    return is_in_reset_mode;
}
*/

    void 
USBComm::sendBuffer(const char *buf, unsigned short int len)
{
    char buf2[len];
    memcpy(buf2,buf,len);
    int ret = usb_bulk_write(m_hdl, 0x1, buf2, len, 0);
    if (ret < 0)
        throw NxtWriteErrorEx();
}


    void
USBComm::sendString(const string str)
{
    return sendBuffer(str.c_str(), str.size());
}


    void
USBComm::recvBuffer(char *buf, unsigned short int len)
{
    int ret = usb_bulk_read(m_hdl, 0x82, buf, len, 2000);//last parameter is timeout
    if (ret < 0)//TODO: need to discriminate error vs timeout
        throw NxtReadErrorEx();
}

