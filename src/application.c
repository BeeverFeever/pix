#include "application.h"
#include "camera.h"

Application app_create(char* name)
{
    Application app = {
        .main_cam = camera_create(),
        .name = name,
        .last_frame = 0,
        .dt = 0,
    };
    return app;
}
