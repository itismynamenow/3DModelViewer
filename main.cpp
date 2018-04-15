#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"

#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "aCamera.h"
#include "aEventManager.h"
#include "aInput.h"
#include "aShader.h"
#include "aRenderMeshData.h"
#include "a3dObjectData.h"
#include "a3dObjectRenderData.h"
#include "aRender3dObjects.h"

#include "test.h"

aInput Input;
aShader Shader;
aCamera Camera;
sf::Clock Clock;
sf::Time Time;
float timeF;
float** globalWeightsArray;
int** globalBoneIDArray;
GLuint *VBO, *bonesBuffer, *weightsBuffer;
int counter=0;
glm::mat4 globalInverse;

using namespace std;


int main()
{
    //Todo: DONE    develop 2 structs: one expands aiMesh and store things as bone maps, transparency, VBOs, aiScene etc and second is container that will be sent to rendering
//
//                          decide if I make game or just engine
//                          decide if I emphasize optimization or functionality
//                 DONE     find what eats most resources (its window (10000mrs)
//                          optimize boneSearch it takes 4mrs so its hard to make it better
//                          learn about vao
//                 DONE     same objects must be drawn after buffers are bind once with uniform updates that requires some object manager that will arrange data efficiently
//                 DONE     fix  memory leaks
//                          model compression for LODs
//                          light
//                          proper material
//                          decide how heavy models should be and textures too
//                          decide what number of unit models should be rendered without lags at 30fps
//
    //this container should contain mesh transform matrix, Id, animationId, animation time etc
    sf::ContextSettings                 CS;
    CS.depthBits=24;

    sf::RenderWindow                    Window(sf::VideoMode(800, 600, 32), "SFML OpenGL",sf::Style::Default, CS);

    aEventManager                       EventManager;

    srand (time(NULL));

    Assimp::Importer                    Importer;

    Camera.viewportSize=glm::vec2(Window.getSize().x,Window.getSize().y);


    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3, 0.3, 1, 1);
    glDepthFunc(GL_LESS);
    glClearDepth(1);

    Shader.addProgram("basicVertex","basicFragment","basicProgram");
    Shader.useProgram("basicProgram");

    aRender3dObjects render;

    a3dObjectData object;
    a3dObjectData object2;
    a3dObjectData object3;

//    object.load3dObject("wolf_dae.dae");
//    object.load3dObject("walking.dae");
//    object.load3dObject("Rigged Hand_dae.dae");
//    object.load3dObject("Handgun_Game_Blender Gamer Engine.dae");
//    object.load3dObject("Dragon_2.5_For_Animations.dae");
//    object.load3dObject("nightwing anim.dae");
    object.load3dObject("ArmyPilot.dae");//100:50mas
//    object.load3dObject("sphere.dae");//100:50mas
//    object.load3dObject("AgentWalk.fbx");//100:50mas

//    object2.load3dObject("walking.dae");//100:50mas
//    object3.load3dObject("nightwing anim.dae");//100:50mas
//    object3.load3dObject("wolf_dae.dae");//100:50mas
//    object2.load3dObject("Dog_By_Sielxm3d.dae");
//    object.load3dObject("walking.dae");
//    object.load3dObject("wolf_dae.dae");
//std::cout<<"Has TC "<<object3.mHasTextureCoords<<std::endl;
//std::cout<<"Has Tex "<<object3.mHasTextures<<std::endl;
//object3.mHasTextures=1;
    int size =64;
    a3dObjectRenderData objectRenderData [size];
//    a3dObjectRenderData objectRenderData2;
//    a3dObjectRenderData objectRenderData3;

    for(int i=0;i<size;i++)
    {
        objectRenderData[i].setObject(&object)     ;
    }


//    objectRenderData2.setObject(&object2)     ;
//    objectRenderData3.setObject(&object3)     ;
//    objectRenderData3.mObjectTransform[3][0]=-3;


    int iSize=8;
    int jSize=8;
    for(int i=0;i<iSize;i++)
    {
        for(int j=0;j<jSize;j++)
        {
            objectRenderData[i*iSize+j].mObjectTransform[3][0]=i;
            objectRenderData[i*iSize+j].mObjectTransform[3][2]=j;
            objectRenderData[i*iSize+j].mAnimationTimeOffset=(rand()%1000)/300.0;
//            std::cout<<objectRenderData[i*iSize+j].mAnimationTimeOffset<<std::endl;
        }
    }



//    for(int i=0;i<object.mScene->mAnimations[0]->mNumChannels;i++)
//    {
//        std::cout<<object.mScene->mAnimations[0]->mChannels[i]->mNodeName.data<<endl;
//    }
    bool pause=false;
    float t1;
    float t2;
    double count=0;
    double sum=0;

    while(Window.isOpen())
    {
        t1=Clock.getElapsedTime().asMicroseconds();

        Time = Clock.getElapsedTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Input.update(&Window);

        EventManager.update(&Window);

        Camera.update();

//        if(Input.p==aInput::key::clicked)
//        {
//            pause=!pause;
//        }
//        if(!pause)
//        {
//            if(object.mScene->mAnimations!=0)
//            timeF=(float)Time.asMilliseconds()/1000.0;
//        }

        for(int i=0;i<size;i++)
        {
             render.pushObjectRenderData( &objectRenderData[i]);
        }
//        render.pushObjectRenderData( &objectRenderData2);
//        render.pushObjectRenderData( &objectRenderData3);

        glUniform1i(Shader.uniformMap["shift"],((int)Time.asSeconds()/10));

        render.shuffleObjects();

        render.render();//25000mrs //110 for 1024 when power is off



//         timeF=0;
//        cout<<((int)Time.asSeconds())<<endl;


//        Window.close();

        Window.display();//10000mrs
//        Sleep(950);
//        t2=(float)Time.asMilliseconds();
//        count++;
//        sum+=t2-t1;
//
//        std::cout<<"Main loop time MSS "<<sum/count<<std::endl;
//        Window.close();
        t2=Clock.getElapsedTime().asMicroseconds();
        count++;
        sum+=t2-t1;
        std::cout<<"Main loop time MrS "<<sum/count<<std::endl;

    }

}
