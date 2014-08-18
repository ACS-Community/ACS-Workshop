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
namespace bluetooth{
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
}
#include "bluecomm.h"

BlueComm::BlueComm(){
    dev_id = bluetooth::hci_get_route(NULL);
    local_sock = bluetooth::hci_open_dev( dev_id );
    if(dev_id<0 || local_sock<0){
        throw NxtNoBluetoothAdapterFoundEx();
    }
    // allocate a socket
    remote_sock = bluetooth::socket(AF_BLUETOOTH, bluetooth::SOCK_STREAM, BTPROTO_RFCOMM);

}
BlueComm::~BlueComm(){
    close(local_sock);//need to access global namespace
}
void
BlueComm::find(){//TODO: impelement method to connect to first bluetooth device found

}
void
BlueComm::connect(const char* mac){
    bluetooth::sockaddr_rc addr = { 0 };
    int status;


    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( mac, &addr.rc_bdaddr );

    // connect to server
    status = bluetooth::connect(remote_sock, (bluetooth::sockaddr *)&addr, sizeof(addr));
    if(status<0){
        throw NxtNotPresentEx();
    }

}
void
BlueComm::connectByName(const char* name){
    int max_rsp = 255, len = 8, flags = IREQ_CACHE_FLUSH, num_rsp;
    char remote_name[248]={0};
    bluetooth::inquiry_info *ii=new bluetooth::inquiry_info[max_rsp];
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
   if(num_rsp < 0){
        throw NxtNotPresentEx();
    }
    for (int i = 0; i < num_rsp; i++) {
        memset(remote_name, 0, sizeof(remote_name));
        hci_read_remote_name(local_sock, &(ii[0].bdaddr), sizeof(remote_name), remote_name, 0);
        if(strcmp(remote_name,name)==0){
            char str[19];
            ba2str(&(ii[0].bdaddr), str );
            //std::cout<<str<<" found"<<endl;
            bluetooth::sockaddr_rc addr = { 0 };
            addr.rc_family = AF_BLUETOOTH;
            addr.rc_channel = (uint8_t) 1;
            bluetooth::str2ba(str, &(addr.rc_bdaddr));

            int status = bluetooth::connect(remote_sock, (bluetooth::sockaddr *)&addr, sizeof(addr));
            if(status<0){
                throw NxtNotPresentEx();
            }
            return;
        }
    }
    throw NxtNotPresentEx();
}
void 
BlueComm::disconnect(){
    close(remote_sock);
}
void 
BlueComm::sendBuffer(const char *buf, unsigned short int len){
    char buf2[len], len2[2];
#ifdef _NXT_LITTLE_ENDIAN
    len2[0]=len&0xff;
    len2[1]=(len>>8)&0xff;
#elif _NXT_BIG_ENDIAN
    len2[0]=(len>>8)&0xff;
    len2[1]=len&0xff;
#endif
    memcpy(buf2,buf,len);
    int status = write(remote_sock, len2, 2);
    if (status < 0)
        throw NxtWriteErrorEx();
    cout<<"Message sent:";
    for(int i=0;i<len;i++){
        printf(" %hhu",buf2[i]);
    }
    cout<<endl;
    status = write(remote_sock, buf2, len);
    if (status < 0)
        throw NxtWriteErrorEx();

}
void 
BlueComm::sendString(const string str){

}
void 
BlueComm::recvBuffer(char *buf, unsigned short int len){
    char len2[2];
    read(remote_sock,len2,2);
    unsigned short int msg_size = len2[0] + 256*len2[1];
    if(len<msg_size){
            throw NxtEx("Buffer too small");
    }
    int bytes_read = read(remote_sock, buf, len);
    if(bytes_read<0 ){
        throw NxtReadErrorEx();
    }
}


