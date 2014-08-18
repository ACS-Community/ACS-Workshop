
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "00:16:53:06:F6:9A";

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    printf("status: %d\n",status);
    // send a message
    if( status == 0 ) {
        char len[2];
        len[0]=6;
        len[1]=0;
        char msg[6];
        msg[0]=0;
        msg[1]=3;
        msg[2]=0;
        msg[3]=2;
        msg[4]=0;
        msg[5]=4;
        status = write(s, len, 2);
        printf("status: %d\n",status);
        status = write(s, msg, 6);
        printf("status: %d\n",status);
        sleep(1);
        msg[0]=0;
        msg[1]=3;
        msg[2]=0;
        msg[3]=1;
        msg[4]=0;
        msg[5]=4;
        status = write(s, len, 2);
        printf("status: %d\n",status);
        status = write(s, msg, 6);
        printf("status: %d\n",status);
        sleep(1);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}
