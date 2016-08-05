#include "capture.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include <gphoto2/gphoto2-camera.h>

using namespace std;


Capture::Capture(Camera* camera, GPContext* context){//:  context(0), camera(0) {
	//gp_camera_new(&camera);
	//context = gp_context_new();
	//gp_context_set_error_func(Capture::context,Capture::ctx_error_func, NULL);
	//gp_context_set_status_func(Capture::context,Capture::ctx_error_func, NULL);
	cout << "Camera init. Can take more than 10 seconds depending on the "                                             
    	<< "memory card's contents (remove card from camera to speed up)." << endl;                                              
    int ret = gp_camera_init(camera, context);                                                                        
    if (ret < GP_OK) {                                                                                                
        printf("No camera auto detected.\n");                                                                         
        gp_camera_free(camera);                                                                                       
        throw ret;                                                                                                     
    }                                             
}

Capture::~Capture(){
	//gp_camera_unref(camera);
	//gp_context_unref(context);
}

void Capture::ctx_error_func(GPContext* context, const char* str, void* data) {
    fprintf(stderr, "%s\n", str);
    fflush(stderr);
}

int Capture::checkCamera(Camera* camera, GPContext* context){
    CameraText text;
    gp_camera_get_summary (camera, &text, context);
    cout << "Summary: " << text.text << endl;
    return 0;
}

int Capture::getConfig(Camera* camera, GPContext* context){
    CameraWidget* window;
    gp_camera_get_config(camera, &window, context);
    cout << "There are " << gp_widget_count_children(window) << " children" << endl;
    return gp_widget_count_children(window);
}


int Capture::capturePicture (Camera* camera, GPContext* context,std::string &filename) {
    int fd, retval;
    CameraFile *file;
    CameraFilePath camera_file_path;
   
    // this was done in the libphoto2 example code, but doesn't seem to be necessary
    // NOP: This gets overridden in the library to /capt0000.jpg
    //snprintf(camera_file_path.folder, 1024, "/");
    //snprintf(camera_file_path.name, 128, "foo.jpg");
   
    // take a shot
    retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
    
    if (retval) {
     // do some error handling, probably return from this function
    }
   
    printf("Pathname on the camera: %s%s\n", camera_file_path.folder, camera_file_path.name);
   
    fd = open(filename.c_str(), O_CREAT | O_WRONLY, 0644);
    // create new CameraFile object from a file descriptor
    retval = gp_file_new_from_fd(&file, fd);
    
    if (retval) {
     // error handling
    }
   
    // copy picture from camera
    retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name, GP_FILE_TYPE_NORMAL, file, context);
    
    if (retval) {
     // error handling
    }
   
    printf("Deleting\n");
    // remove picture from camera memory
    retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name, context);
    
    if (retval) {
     // error handling
    }
   
    // free CameraFile object
    gp_file_free(file);
   
    // Code from here waits for camera to complete everything.
    // Trying to take two successive captures without waiting
    // will result in the camera getting randomly stuck.
    int waittime = 200;
    CameraEventType type;
    void *data;
   
    printf("Wait for events from camera\n");
    while(1) {
        retval = gp_camera_wait_for_event(camera, waittime, &type, &data, context);
        if(type == GP_EVENT_TIMEOUT) {
            break;
        }
        else if (type == GP_EVENT_CAPTURE_COMPLETE) {
            printf("Capture completed\n");
            waittime = 100;
        }
        else if (type != GP_EVENT_UNKNOWN) {
            printf("Unexpected event received from camera: %d\n", (int)type);
        }
    }
    return fd;
}

const string Capture::isoValues[5]= {
    "100",
    "200",
    "400",
    "800",
    "1600"
};

void Capture::setIso(Camera* camera, GPContext* context, const char* value){
    // Set up iso
    CameraWidget *widgettree, *xwidget;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "iso", &xwidget);
    cout << "Setting Iso to " << value << endl;
    gp_widget_set_value (xwidget, value);
    gp_camera_set_config(camera, widgettree, context);
}

std::string Capture::getIso(Camera* camera, GPContext* context){
    CameraWidget *widgettree, *xwidget;
    string myValue;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "iso", &xwidget);
    //cout << "Iso has " << gp_widget_count_choices(xwidget) << " options" <<endl;
    gp_widget_get_value(xwidget, &myValue);
    //cout << "Iso is " << myValue.c_str() << endl;
    return myValue.c_str();
}

const string Capture::shutterSpeedValues[52]= {
    "30",
    "25",
    "20",
    "15",
    "13",
    "10",
    "8",
    "6",
    "5",
    "4",
    "3.2",
    "2.5",
    "2",
    "1.6",
    "1.3",
    "1",
    "0.8",
    "0.6",
    "0.5",
    "0.4",
    "0.3",
    "1/4",
    "1/5",
    "1/6",
    "1/8",
    "1/10",
    "1/13",
    "1/15",
    "1/20",
    "1/25",
    "1/30",
    "1/40",
    "1/50",
    "1/60",
    "1/80",
    "1/100",
    "1/125",
    "1/160",
    "1/200",
    "1/250",
    "1/320",
    "1/400",
    "1/500",
    "1/640",
    "1/800",
    "1/1000",
    "1/1250",
    "1/1600",
    "1/2000",
    "1/2500",
    "1/3200",
    "1/4000"
};


void Capture::setShutterSpeed(Camera *camera, GPContext *context, const char* value){
    // Set up shutterspeed
    CameraWidget *widgettree, *xwidget;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "shutterspeed", &xwidget);
    //cout << "Shutter has " << gp_widget_count_choices(xwidget) << " options" <<endl;
    gp_widget_set_value (xwidget, value);
    gp_camera_set_config(camera, widgettree, context);
}

std::string Capture::getShutterSpeed(Camera *camera, GPContext *context){
    CameraWidget *widgettree, *xwidget;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "shutterspeed", &xwidget);
    string myValue;
    gp_widget_get_value(xwidget, &myValue);
    //cout << "Shutter is " << myValue.c_str() << endl;
    return myValue.c_str();
}

