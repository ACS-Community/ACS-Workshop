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

#include "opcodes.h"
#include <iostream>

string
ErrorCodes::getErrorMessage(char code){
    switch(code){
        case 0x20:
            return "Pending communication transaction in progress";
        case 0x40:
            return "Specified mailbox queue is empty";
        case 0xbd:
            return "Request failed (i.e. specified file not found)";
        case 0xbe:
            return "Unknown command opcode";
        case 0xbf:
            return "Insane packet";
        case 0xc0:
            return "Data contains out-of-range values";
        case 0xdd:
            return "Communication bus error";
        case 0xde:
            return "No free memory in communication buffer";
        case 0xdf:
            return "Specified channel/connection is not valid";
        case 0xe0:
            return "Specified channel/connection not configured or busy";
        case 0xec:
            return "No active program";
        case 0xed:
            return "Illegal size specified";
        case 0xee:
            return "Illegal mailbox queue ID specified";
        case 0xef:
            return "Attempted to access invalid field of a structure";
        case 0xf0:
            return "Bad input or output specified";
        case 0xfb:
            return "Insufficient memory available";
        case 0xff:
            return "Bad arguments";
        default:
            return "Unknown error code";
    }
}



Opcodes::Opcodes(Comms *nxt){
    this->nxt = nxt;
}

void
Opcodes::playTone(unsigned short int freq, unsigned short int time, bool answer, unsigned char *status){
    Message *msg;
    if(answer){
        msg=new Message(0x0,0x3);
    }else{
        msg=new Message(0x80,0x3);
    }
    msg->addU16(freq);
    msg->addU16(time);
    nxt->sendBuffer(msg->getMsg(),msg->getLen());

    if(answer){
        try{
            char buf[3];
            nxt->recvBuffer(buf, 3);
            if(buf[0]!=0x02 || buf[1]!=0x03){
                throw NxtEx("Answer received is not the expected one");
            }
            *status=buf[2];
        }catch(NxtEx &ex){
            cout<<ex.toString()<<endl;
        }catch(...){
            cout<<"Malformed answer from brick"<<endl;
        }
    }
    usleep(time*1000);
}

unsigned short int 
Opcodes::getBatteryLevel(){
    Message msg(0,0x0b);
    nxt->sendBuffer(msg.getMsg(),msg.getLen());

    char buf[5];
    nxt->recvBuffer(buf, 5);
    unsigned short int bat= buf[3] + 256*buf[4];

    return bat;
}

void 
Opcodes::setOutputState(unsigned char port, char power, unsigned char mode, unsigned char regMode, char turnRatio, unsigned char runState, unsigned int tachoLimit, bool answer, unsigned char *status){
    Message *msg;
    if(answer){
        msg=new Message(0x0,0x4);
    }else{
        msg=new Message(0x80,0x4);
    }
    msg->addU8(port);
    msg->addS8(power);
    msg->addU8(mode);
    msg->addU8(regMode);
    msg->addS8(turnRatio);
    msg->addU8(runState);
    msg->addU32(tachoLimit);
    nxt->sendBuffer(msg->getMsg(),msg->getLen());

    if(answer){
        try{
            char buf[3];
            nxt->recvBuffer(buf, 3);
            if(buf[0]!=0x02 || buf[1]!=0x04){
                throw NxtEx("Answer received is not the expected one");
            }
            *status=buf[2];
        }catch(NxtEx &ex){
            cout<<ex.toString()<<endl;
        }catch(...){
            cout<<"Malformed answer from brick"<<endl;
        }
    }

}
MotorState 
Opcodes::getOutputState(unsigned char port){
    Message *msg;
    msg=new Message(0x0,0x6);
    msg->addU8(port);
    nxt->sendBuffer(msg->getMsg(),msg->getLen());
    char buf[25];
    nxt->recvBuffer(buf, 25);
    if(buf[0]!=0x02 || buf[1]!=0x06){
        throw NxtEx("Answer received is not the expected one");
    }
    if(buf[2]!=0){
        throw NxtEx(ErrorCodes::getErrorMessage(buf[2]));
    }
    MotorState ret;
    ret.port=buf[3];
    ret.power=buf[4];
    ret.mode=buf[5];
    ret.regMode=buf[6];
    ret.turnRatio=buf[7];
    ret.runState=buf[8];
    ret.tachoLimit=Message::bytes2U32(&(buf[9]));
    ret.tachoCount=Message::bytes2S32(&(buf[13]));
    ret.blockTachoCount=Message::bytes2S32(&(buf[17]));
    ret.rotationCount=Message::bytes2S32(&(buf[21]));
    return ret;
}
void 
Opcodes::setInputMode(unsigned char port, SensorType type, SensorMode mode, bool answer, unsigned char *status){
    Message *msg;
    if(answer){
        msg=new Message(0x0,0x5);
    }else{
        msg=new Message(0x80,0x5);
    }
    msg->addU8(port);
    msg->addU8(type);
    msg->addU8(mode);
    nxt->sendBuffer(msg->getMsg(),msg->getLen());

    if(answer){
        try{
            char buf[3];
            nxt->recvBuffer(buf, 3);
            if(buf[0]!=0x02 || buf[1]!=0x05){
                throw NxtEx("Answer received is not the expected one");
            }
            *status=buf[2];
        }catch(NxtEx &ex){
            cout<<ex.toString()<<endl;
        }catch(...){
            cout<<"Malformed answer from brick"<<endl;
        }
    }
}
SensorState 
Opcodes::getInputValues(unsigned char port){
    Message *msg;
    msg=new Message(0x0,0x7);
    msg->addU8(port);
    nxt->sendBuffer(msg->getMsg(),msg->getLen());
    char buf[16];
    nxt->recvBuffer(buf, 16);
    if(buf[0]!=0x02 || buf[1]!=0x07){
        throw NxtEx("Answer received is not the expected one");
    }
    if(buf[2]!=0){
        throw NxtEx(ErrorCodes::getErrorMessage(buf[2]));
    }
    SensorState ret;
    ret.port=buf[3];
    (buf[4]>0) ? ret.valid=true : ret.valid=false;
    (buf[5]>0) ? ret.calibrated=true : ret.calibrated=false;
    ret.type=SensorType((int)buf[6]);
    ret.mode=SensorMode((int)buf[7]);
    ret.rawValue=Message::bytes2U16(&(buf[8]));
    ret.normalizedValue=Message::bytes2U16(&(buf[10]));
    ret.scaledValue=Message::bytes2S16(&(buf[12]));
    ret.calibratedValue=Message::bytes2S16(&(buf[14]));
    return ret;
}
    
void 
Opcodes::resetMotorPosition(unsigned char port, bool relative, bool answer, unsigned char *status){
    Message *msg;
    if(answer){
        msg=new Message(0x0,0xa);
    }else{
        msg=new Message(0x80,0xa);
    }
    msg->addU8(port);
    if(relative){
        msg->addU8(1);
    }else{
        msg->addU8(0);
    }
    nxt->sendBuffer(msg->getMsg(),msg->getLen());

    if(answer){
        try{
            char buf[3];
            nxt->recvBuffer(buf, 3);
            if(buf[0]!=0x02 || buf[1]!=0x0a){
                throw NxtEx("Answer received is not the expected one");
            }
            *status=buf[2];
        }catch(NxtEx &ex){
            cout<<ex.toString()<<endl;
        }catch(...){
            cout<<"Malformed answer from brick"<<endl;
        }
    }
}



