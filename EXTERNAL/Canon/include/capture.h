#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include <gphoto2/gphoto2-camera.h>


class Capture
{
    public:
    Capture(Camera* camera, GPContext* context);
    //Capture();
    virtual ~Capture();
    static void ctx_error_func(GPContext* context, const char* str, void* data);
    int checkCamera(Camera* camera, GPContext* context);
    int getConfig(Camera* camera, GPContext* context);
    int capturePicture (Camera* camera, GPContext* context,std::string &filename);
    static const std::string isoValues[5];
    void setIso(Camera *camera, GPContext *context, const char* value);
    std::string getIso(Camera *camera, GPContext *context);
    static const std::string shutterSpeedValues[52];
    void setShutterSpeed(Camera *camera, GPContext *context, const char* value);
    std::string getShutterSpeed(Camera *camera, GPContext *context);
    
    GPContext* context;
    Camera* camera;
};
