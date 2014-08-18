#include "lpiPropsDevIO.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <string.h>

lpiPropsDevIO::lpiPropsDevIO(int in_fd, int id) 
{
	control_id = id;
	fd = in_fd;
}

lpiPropsDevIO::~lpiPropsDevIO(){
}

void lpiPropsDevIO::write(const CORBA::Long &value, ACS::Time &timestap)
                      			throw (ACSErr::ACSbaseExImpl)
{
	struct v4l2_queryctrl queryctrl;
        struct v4l2_control control;

        memset (&queryctrl, 0, sizeof (queryctrl));
        queryctrl.id = control_id;

        if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
                if (errno != EINVAL) {
                        perror ("VIDIOC_QUERYCTRL");
                        exit (EXIT_FAILURE);
                } else {
                        printf ("The control id %d is not supported\n",control_id);
                }
        } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
                        printf ("The control id %d is not supported\n",control_id);
        } else {
                memset (&control, 0, sizeof (control));
                control.id = control_id;
                control.value = value;
                if (-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
                        perror ("VIDIOC_S_CTRL");
                        exit (EXIT_FAILURE);
                }
        }

}

CORBA::Long lpiPropsDevIO::read(ACS::Time &timestamp)
              		throw (ACSErr::ACSbaseExImpl)
{
	struct v4l2_queryctrl queryctrl;
        struct v4l2_control control;

        memset (&queryctrl, 0, sizeof (queryctrl));
        queryctrl.id = control_id;

        if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
                if (errno != EINVAL) {
                        perror ("VIDIOC_QUERYCTRL");
                        exit (EXIT_FAILURE);
                } else {
                        printf ("The control id %d is not supported\n",control_id);
                }
        } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
                printf ("The control id %d is not supported\n",control_id);
        } else {
                memset (&control, 0, sizeof (control));
                control.id = control_id;
                if ( -1 == ioctl (fd, VIDIOC_G_CTRL, &control) ){
                        perror("VIDIOC_G_CTRL");
                        exit(EXIT_FAILURE);
                }
        }
	return (CORBA::Long) control.value;
}

