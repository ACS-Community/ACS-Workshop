#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <gphoto2/gphoto2-camera.h>

class Iso{
    public:
        
        Iso();
 	~Iso(); 
        static const std::string isoValues[5];
        void setIso(Camera *camera, GPContext *context, const char* value);
        std::string getIso(Camera *camera, GPContext *context);
	
	GPContext* context;
    	Camera* camera;


};

