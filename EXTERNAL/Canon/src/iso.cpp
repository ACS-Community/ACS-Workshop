#include "capture.h"
#include "iso.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <gphoto2/gphoto2-camera.h>

using namespace std;


Iso::Iso():  context(0), camera(0) {
        gp_camera_new(&camera);
        context = gp_context_new();
        gp_context_set_error_func(capture.context,Capture::ctx_error_func, NULL);
        gp_context_set_status_func(capture.context,Capture::ctx_error_func, NULL);
    int ret = gp_camera_init(camera, context);
    if (ret < GP_OK) {
        printf("No camera auto detected.\n");
        gp_camera_free(camera);
        throw ret;
    }
}

Iso::~Iso(){
        gp_camera_unref(camera);
        gp_context_unref(context);
}

const string Iso::isoValues[5]= {
    "100",
    "200",
    "400",
    "800",
    "1600"
};

void Iso::setIso(Camera* camera, GPContext* context, const char* value){
    // Set up iso
    CameraWidget *widgettree, *xwidget;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "iso", &xwidget);
    cout << "Setting Iso to " << value << endl;
    gp_widget_set_value (xwidget, value);
    gp_camera_set_config(camera, widgettree, context);
}

std::string Iso::getIso(Camera* camera, GPContext* context){
    CameraWidget *widgettree, *xwidget;
    string myValue;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "iso", &xwidget);
    cout << "Iso has " << gp_widget_count_choices(xwidget) << " options" <<endl;
    gp_widget_get_value(xwidget, &myValue);
    cout << "Iso is " << myValue.c_str() << endl;
    return myValue;
}

