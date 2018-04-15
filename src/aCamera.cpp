#include "aCamera.h"

aCamera::aCamera()
{
    //ctor
}

aCamera::~aCamera()
{
    //dtor
}

void aCamera::update()
{
    if(Input.leftMouseButton!=aInput::key::clicked&&Input.leftMouseButton!=aInput::key::pressed)
    {
        inViewport=false;
    }
    if(Input.leftMouseButton==aInput::key::clicked)
    {
        if(Input.mouseX<viewportSize.x+1)
        {
            oldRotationalAngle=glm::vec2(Input.mouseX,Input.mouseY);
            inViewport=true;
        }
    }
    if((Input.leftMouseButton==aInput::key::pressed||Input.leftMouseButton==aInput::key::clicked)&&inViewport)
    {
         rotationAngle=glm::vec2(Input.mouseX,Input.mouseY);
         angleX+=(-rotationAngle.x+oldRotationalAngle.x)/viewportSize.x*cameraRotationalSpeed;
         angleY+=(-rotationAngle.y+oldRotationalAngle.y)/viewportSize.y*cameraRotationalSpeed;
    }

     direction=glm::vec3( glm::cos(glm::radians(angleY))* glm::sin(glm::radians(angleX)),
                          glm::sin(glm::radians(angleY)),
                          glm::cos(glm::radians(angleY)) * glm::cos(glm::radians(angleX)));

     right = glm::vec3(glm::sin(glm::radians(angleX) - 3.14f/2.0f),
                       0,
                       cos(glm::radians(angleX) - 3.14f/2.0f));

     up = glm::cross(right, direction);

     if(Input.mouseWheel>0)
        position+=glm::normalize(direction)*(cameraSpeed*2);
     if(Input.mouseWheel<0)
        position+=glm::normalize(direction)*(-cameraSpeed*2);
     if(Input.w==aInput::key::pressed||Input.w==aInput::key::clicked)
        position-=glm::normalize(up)*-cameraSpeed;
     if(Input.s==aInput::key::pressed||Input.s==aInput::key::clicked)
         position-=glm::normalize(up)*cameraSpeed;
     if(Input.a==aInput::key::pressed||Input.a==aInput::key::clicked)
        position-=glm::normalize(right)*cameraSpeed;
     if(Input.d==aInput::key::pressed||Input.d==aInput::key::clicked)
        position-=glm::normalize(right)*-cameraSpeed;

     if(Input.plus==aInput::key::pressed||Input.plus==aInput::key::clicked)
        cameraSpeed*=1.2;
     if(Input.minus==aInput::key::pressed||Input.minus==aInput::key::clicked)
        cameraSpeed/=1.2;

     CameraMatrix = glm::perspective(glm::radians(fov),aspect,closePlane,farPlane)*
     glm::lookAt(position,position+direction,up);

     oldRotationalAngle=rotationAngle;
}
