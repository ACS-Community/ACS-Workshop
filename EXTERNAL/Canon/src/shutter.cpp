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

enum shutterSpeedIndexes{
    s0 = 0,
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    s12,
    s13,
    s14,
    s15,
    s16,
    s17,
    s18,
    s19,
    s20,
    s21,
    s22,
    s23,
    s24,
    s25,
    s26,
    s27,
    s28,
    s29,
    s30,
    s31,
    s32,
    s33,
    s34,
    s35,
    s36,
    s37,
    s38,
    s39,
    s40,
    s41,
    s42,
    s43,
    s44,
    s45,
    s46,
    s47,
    s48,
    s49,
    s50,
    s51
};

//const char* shutterSpeedsValues[]{
string shutterSpeedValues[52]= {
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

void setShutterSpeed(Camera *camera, GPContext *context, const char* value){
    // Set up shutterspeed
    CameraWidget *widgettree, *xwidget;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "shutterspeed", &xwidget);
    cout << "Shutter has " << gp_widget_count_choices(xwidget) << " options" <<endl;
    gp_widget_set_value (xwidget, value);
    gp_camera_set_config(camera, widgettree, context);
}

string getShutterSpeed(Camera *camera, GPContext *context){
    CameraWidget *widgettree, *xwidget;
    gp_camera_get_config(camera, &widgettree, context);
    gp_widget_get_child_by_name (widgettree, "shutterspeed", &xwidget);
    string myValue;
    gp_widget_get_value(xwidget, &myValue);
    cout << "Shutter is " << myValue.c_str() << endl;
    return myValue;
}



int main(){
    gp_camera_new (&camera);
    context = gp_context_new();
    // set callbacks for camera messages
    gp_context_set_error_func(context, ctx_error_func, NULL);
    gp_context_set_status_func(context, ctx_error_func, NULL);
             
//    // Set up shutterspeed
    for (int i = 0; i < 52; i++){
        cout << " Setting option " << i << " => " << shutterSpeedValues[i]  << endl;
        setShutterSpeed(camera, context, shutterSpeedValues[i].c_str());
        sleep(1);
        getShutterSpeed(camera, context);
    }
    // close camera
    gp_camera_unref(camera);
    gp_context_unref(context);
    return 0;
   
}
