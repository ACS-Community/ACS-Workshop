#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <string.h>

#include "lpiFrameDevIO.h"

lpiFrameDevIO::lpiFrameDevIO(int in_fd, char *deviceName) { // throw (UOSErr::CannotOpenDeviceEx){
	fd = in_fd;

	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;

	CLEAR (cropcap);
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == ioctl (fd, VIDIOC_CROPCAP, &cropcap))
	{
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  //same as the cropcap.type
		//crop.c = cropcap.defrect; //set to default cropping rectangle
		crop.c.top = 0;
		crop.c.left = 0;
		crop.c.width = 640 * 24;
		crop.c.height = 480 * 24;

		//to set the current cropping rectangle
		if (-1 == ioctl (fd, VIDIOC_S_CROP, &crop))
		{
			if(errno == EINVAL)
				fprintf (stderr, "Cropping not supported for the %s\n", deviceName);
			else
				fprintf (stderr, "There is a problem setting the cropping limits of the %s\n", deviceName);
		}
	}
	else
	{
		fprintf (stderr, "There is a problem quering the cropping limits of the %s\n", deviceName);
	}

	//to set the format
	CLEAR (fmt);
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SN9C10X;
	//fmt.fmt.pix.pixelformat =  V4L2_PIX_FMT_SBGGR8;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

	//to negotiate the format
	if (-1 == ioctl (fd, VIDIOC_S_FMT, &fmt));

	else ACS_SHORT_LOG((LM_INFO,"lpiFrameDevIO::lpiFrameDevIO: Video device ready"));

	framesize = fmt.fmt.pix.sizeimage;
	sonix_unknown = 0;
	init_done = 0;
}

lpiFrameDevIO::~lpiFrameDevIO() {

	ACS_SHORT_LOG((LM_INFO,"lpiFrameDevIO::~lpiFrameDevIO: Device closed :Device closed :D"));
}

bool lpiFrameDevIO::initializeValue() {
	return true;
}

ACS::longSeq lpiFrameDevIO::read(ACS::Time &timestamp) throw (ACSErr::ACSbaseExImpl) {

	unsigned char *d      = new unsigned char[640 * 480 * 3];
	unsigned char *s      = new unsigned char[640 * 480];
	unsigned char *buffer = new unsigned char[framesize];

	if (!buffer || !d || !s) {
		ACS_SHORT_LOG((LM_ERROR,"lpiFrameDevIO::read: Out of memory!"));
	}

	if (-1 == ::read (fd, buffer, framesize))
	{
		if( errno == EAGAIN )
			return 0;
	}

	ACS_SHORT_LOG((LM_INFO,"lpiFrameDevIO::read: Obtained a frame"));
	sonix_decompress_init();
	sonix_decompress(640,480,buffer,s);
	bayer2rgb24(d,buffer,640,480);
	ACS_SHORT_LOG((LM_INFO,"lpiFrameDevIO::read: Decompressed frame"));

	ACS::longSeq ret;
	ret.length(640*480*3);
	for(int i=0; i!=(640*480*3); i++)
		ret[i] = (int)d[i];

	delete [] buffer;
	delete [] s;
	delete [] d;
	return ret;
}

void lpiFrameDevIO::write(const ACS::longSeq &value, ACS::Time &timestamp) throw (ACSErr::ACSbaseExImpl){
	ACS_SHORT_LOG((LM_ERROR, "lpiFrameDevIO::write: This method should never be called!"));
	return;
}

int lpiFrameDevIO::sonix_decompress (int width, int height, unsigned char *inp, unsigned char *outp)
{
	int row, col;
	int val;
	int bitpos;
	unsigned char code;
	unsigned char *addr;

	if (!init_done)
	{
		 /* do sonix_decompress_init first! */
		 sonix_decompress_init();
	}

	bitpos = 0;
	for (row = 0; row < height; row++)
	{

		col = 0;
	
		/* first two pixels in first two rows are stored as raw 8-bit */
		if (row < 2)
		{
			addr = inp + (bitpos >> 3);
			code = (addr[0] << (bitpos & 7)) | (addr[1] >> (8 - (bitpos & 7)));
			bitpos += 8;
			*outp++ = code;
	
			addr = inp + (bitpos >> 3);
			code = (addr[0] << (bitpos & 7)) | (addr[1] >> (8 - (bitpos & 7)));
			bitpos += 8;
			*outp++ = code;
	
			col += 2;
		}
	
		while (col < width)
		{
			/* get bitcode from bitstream */
			addr = inp + (bitpos >> 3);
			code = (addr[0] << (bitpos & 7)) | (addr[1] >> (8 - (bitpos & 7)));
	
			/* update bit position */
			bitpos += table[code].len;
	
			/* update code statistics */
			sonix_unknown += table[code].unk;
	
			/* calculate pixel value */
			val = table[code].val;
			if (!table[code].is_abs)
			{
				/* value is relative to top and left pixel */
				if (col < 2)
				{
					 /* left column: relative to top pixel */
					 val += outp[-2 * width];
				}
				else if (row < 2)
				{
					 /* top row: relative to left pixel */
					 val += outp[-2];
				}
				else
				{
					 /* main area: average of left pixel and top pixel */
					 val += (outp[-2] + outp[-2 * width]) / 2;
				}
			}
	
			/* store pixel */
			*outp++ = CLAMP (val);
			col++;
		}
	}

	return 0;
}

void lpiFrameDevIO::sonix_decompress_init (void)
{
	int i;
	int is_abs, val, len, unk;
	
	for (i = 0; i < 256; i++) {
		is_abs = 0;
		val = 0;
		len = 0;
		unk = 0;
		if ((i & 0x80) == 0) {
			/* code 0 */
			val = 0;
			len = 1;
		}
		else if ((i & 0xE0) == 0x80){
			/* code 100 */
			val = +4;
			len = 3;
		}
		else if ((i & 0xE0) == 0xA0){
			/* code 101 */
			val = -4;
			len = 3;
		}
		else if ((i & 0xF0) == 0xD0)
		{
			/* code 1101 */
			val = +11;
			len = 4;
		}
		else if ((i & 0xF0) == 0xF0){
			/* code 1111 */
			val = -11;
			len = 4;
		}
		else if ((i & 0xF8) == 0xC8){
			/* code 11001 */
			val = +20;
			len = 5;
		}
		else if ((i & 0xFC) == 0xC0){
			  /* code 110000 */
			  val = -20;
			  len = 6;
		}
		else if ((i & 0xFC) == 0xC4){
			/* code 110001xx: unknown */
			val = 0;
			len = 8;
			unk = 1;
		}
		else if ((i & 0xF0) == 0xE0){
			/* code 1110xxxx */
			is_abs = 1;
			val = (i & 0x0F) << 4;
			len = 8;
		}
		table[i].is_abs = is_abs;
		table[i].val = val;
		table[i].len = len;
		table[i].unk = unk;
	}

	sonix_unknown = 0;
	init_done = 1;
}

void lpiFrameDevIO::bayer2rgb24 (unsigned char *dst, unsigned char *src, long int WIDTH,
	     long int HEIGHT)
{
  long int i;
  unsigned char *rawpt, *scanpt;
  long int size;

  rawpt = src;
  scanpt = dst;
  size = WIDTH * HEIGHT;

  for (i = 0; i < size; i++)
    {
      if ((i / WIDTH) % 2 == 0)
	{
	  if ((i % 2) == 0)
	    {
	      /* B */
	      if ((i > WIDTH) && ((i % WIDTH) > 0))
		{
		  *scanpt++ = (*(rawpt - WIDTH - 1) + *(rawpt - WIDTH + 1) + *(rawpt + WIDTH - 1) + *(rawpt + WIDTH + 1)) / 4;	/* R */
		  *scanpt++ = (*(rawpt - 1) + *(rawpt + 1) + *(rawpt + WIDTH) + *(rawpt - WIDTH)) / 4;	/* G */
		  *scanpt++ = *rawpt;	/* B */
		}
	      else
		{
		  /* first line or left column */
		  *scanpt++ = *(rawpt + WIDTH + 1);	/* R */
		  *scanpt++ = (*(rawpt + 1) + *(rawpt + WIDTH)) / 2;	/* G */
		  *scanpt++ = *rawpt;	/* B */
		}
	    }
	  else
	    {
	      /* (B)G */
	      if ((i > WIDTH) && ((i % WIDTH) < (WIDTH - 1)))
		{
		  *scanpt++ = (*(rawpt + WIDTH) + *(rawpt - WIDTH)) / 2;	/* R */
		  *scanpt++ = *rawpt;	/* G */
		  *scanpt++ = (*(rawpt - 1) + *(rawpt + 1)) / 2;	/* B */
		}
	      else
		{
		  /* first line or right column */
		  *scanpt++ = *(rawpt + WIDTH);	/* R */
		  *scanpt++ = *rawpt;	/* G */
		  *scanpt++ = *(rawpt - 1);	/* B */
		}
	    }
	}
      else
	{
	  if ((i % 2) == 0)
	    {
	      /* G(R) */
	      if ((i < (WIDTH * (HEIGHT - 1))) && ((i % WIDTH) > 0))
		{
		  *scanpt++ = (*(rawpt - 1) + *(rawpt + 1)) / 2;	/* R */
		  *scanpt++ = *rawpt;	/* G */
		  *scanpt++ = (*(rawpt + WIDTH) + *(rawpt - WIDTH)) / 2;	/* B */
		}
	      else
		{
		  /* bottom line or left column */
		  *scanpt++ = *(rawpt + 1);	/* R */
		  *scanpt++ = *rawpt;	/* G */
		  *scanpt++ = *(rawpt - WIDTH);	/* B */
		}
	    }
	  else
	    {
	      /* R */
	      if (i < (WIDTH * (HEIGHT - 1)) && ((i % WIDTH) < (WIDTH - 1)))
		{
		  *scanpt++ = *rawpt;	/* R */
		  *scanpt++ = (*(rawpt - 1) + *(rawpt + 1) + *(rawpt - WIDTH) + *(rawpt + WIDTH)) / 4;	/* G */
		  *scanpt++ = (*(rawpt - WIDTH - 1) + *(rawpt - WIDTH + 1) + *(rawpt + WIDTH - 1) + *(rawpt + WIDTH + 1)) / 4;	/* B */
		}
	      else
		{
		  /* bottom line or right column */
		  *scanpt++ = *rawpt;	/* R */
		  *scanpt++ = (*(rawpt - 1) + *(rawpt - WIDTH)) / 2;	/* G */
		  *scanpt++ = *(rawpt - WIDTH - 1);	/* B */
		}
	    }
	}
      rawpt++;
    }

}
