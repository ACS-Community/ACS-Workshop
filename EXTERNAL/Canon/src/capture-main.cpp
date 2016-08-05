#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include <gphoto2/gphoto2-camera.h>
#include "capture.h"

using namespace std;

Camera *camera;
GPContext *context;


int main (int argc, char *argv[]) {
    gp_camera_new(&camera);
    context = gp_context_new();
    Capture capture = Capture (camera, context);
    capture.checkCamera(camera, context);  
    int i;
    
    // take nShots shots
    char filename[256];
    int const nShots = 1;
    int fileDesc;
   
    // do some capturing
    for (i = 0; i < nShots; i++) {
        snprintf(filename, 256, "capt-%04d.jpg", i);
        printf("Capturing to file %s\n", filename);
	std::string name(filename);
        fileDesc = capture.capturePicture(camera, context,name);
        close(fileDesc);
	sleep(2);
    }

    //test iso settings: from 0 to 4
    for (i = 0 ; i<5; i++){
        string message = capture.getIso(camera, context);
        cout << "iso set to " ;
        cout << message << endl ;  
        capture.setIso(camera, context, capture.isoValues[i].c_str());
        sleep(2);
   }

  
    //test shutter settings: from 0 to 51
    for (i = 0 ; i<52; i++){
        string message = capture.getShutterSpeed(camera, context);
        cout << "shutter set to ";
        cout << message << endl ;  
        capture.setShutterSpeed(camera, context, capture.shutterSpeedValues[i].c_str());
        sleep(2);
   }

//    fileDesc =  open(filename, O_RDONLY); 
//    char* buffer;
//    buffer = (char*) malloc(1024);
//    size_t offset = 0; 
//    size_t bytes_read; 
//    do {
//        /* Read the next line's worth of bytes.  */ 
//        bytes_read = read (fileDesc, buffer, sizeof (buffer)); 
//        /* Print the offset in the file, followed by the bytes themselves.  */ 
//        printf ("0x%06x : ", offset); 
//        for (i = 0; i < bytes_read; ++i) 
//          printf ("%02x ", buffer[i]); 
//        printf ("\n"); 
//        /* Keep count of our position in the file.  */ 
//        offset += bytes_read; 
//      } 
//      while (bytes_read == sizeof (buffer)); 
      gp_camera_unref(camera);
      gp_context_unref(context);
    return 0;
}
