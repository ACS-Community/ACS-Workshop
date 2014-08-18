/*
 * CSRG - UTFSM Computer Systems Research Group
 * ACS-UTFSM Team
 */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "SCTdefs.h"

#define LEGO_TOWER_SET_READ_TIMEOUT _IOW('u', 0xc8, int)
#define LEGO_TOWER_SET_WRITE_TIMEOUT _IOW('u', 0xc9, int)
#define LEGO_TOWER_READ_TIMEOUT_VALUE 100
#define BUF_SIZE 40

short int getVariable(int fd, unsigned char variable);
int setVariable(int fd, int variable, short int value);
int openLego(char *dev);


