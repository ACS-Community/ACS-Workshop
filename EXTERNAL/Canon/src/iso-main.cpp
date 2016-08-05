#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <gphoto2/gphoto2-camera.h>

using namespace std;

Camera *camera;
GPContext *context;
static void ctx_error_func(GPContext* context, const char* str, void* data) {
    fprintf(stderr, "%s\n", str);
    fflush(stderr);
}

string isoValues[5]= {
    "100",
    "200",
    "400",
    "800",
    "1600"
};

void setIso(Camera *camera, GPContext *context, const char* value){
    // Set up iso
    CameraWidget *widgettree, *xwidget;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "iso", &xwidget);
    gp_widget_set_value (xwidget, value);
    gp_camera_set_config(camera, widgettree, context);
}

string getIso(Camera *camera, GPContext *context){
    CameraWidget *widgettree, *xwidget;
    string myValue;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "iso", &xwidget);
    cout << "Iso has " << gp_widget_count_choices(xwidget) << " options" <<endl;
    gp_widget_get_value(xwidget, &myValue);
    cout << "Iso is " << myValue.c_str() << endl;
    return myValue;
}


int main(){
    gp_camera_new (&camera);
    context = gp_context_new();
    // set callbacks for camera messages
    gp_context_set_error_func(context, ctx_error_func, NULL);
    gp_context_set_status_func(context, ctx_error_func, NULL);
             
    for (int i = 0; i < 5; i++){
        cout << " Setting option " << i << " => " << isoValues[i]  << endl;
        setIso(camera, context, isoValues[i].c_str());
        sleep(1);
        getIso(camera, context);
    }
    // close camera
    gp_camera_unref(camera);
    gp_context_unref(context);
    return 0;
   
}
