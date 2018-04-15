#ifndef ACAMERA_H
#define ACAMERA_H

#include "SFML/OpenGL.hpp"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "aInput.h"

extern aInput                       Input;

class aCamera
{
    public:
        aCamera();
        virtual ~aCamera();

        float                               fov=60,
                                            farPlane=1000,
                                            closePlane=0.1,
                                            aspect=1,
                                            cameraSpeed=0.05,
                                            cameraRotationalSpeed=180,
                                            angleY=0,
                                            angleX=0;

        bool                                inViewport=false;

        glm::vec2                           rotationAngle=glm::vec2(0,0),
                                            oldRotationalAngle=glm::vec2(0,0),
                                            mouseOffset,
                                            viewportSize,
                                            viewportPosition;

        glm::vec3                           up=glm::vec3(0.0f, 1.0f, 0.0f),
                                            position=glm::vec3(0.0f,0.0f,-2.0f),
                                            direction=glm::vec3(0.0f,0.0f,1.0f),
                                            cameraMove=glm::vec3(0.0f,0.0f,0.0f),
                                            right;

        glm::mat4                           CameraMatrix;

        void                                update ();


    protected:

    private:
};

#endif // ACAMERA_H
