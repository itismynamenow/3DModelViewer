#ifndef ARENDER3DOBJECTS_H
#define ARENDER3DOBJECTS_H
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "SFML/Graphics.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "a3dObjectRenderData.h"
#include "aCamera.h"
#include "aShader.h"

extern aShader Shader;
extern aCamera Camera;
extern float timeF;
extern sf::Clock Clock;


class aRender3dObjects
{
    public:
        aRender3dObjects();
        virtual ~aRender3dObjects();

        a3dObjectRenderData*                                                                            mObjectData;
        //ar[num of dif objects][num of same obj][num of meshes][num of bones]

        glm::mat4****                                                                                   mBoneUniformGlobal;
        glm::mat4**                                                                                     mBoneUniform;
        vector<a3dObjectRenderData*>                                                                    mObjectRenderDataVector;
        vector<vector<a3dObjectRenderData*>>                                                            mShuffledObjects;

        float t1,t2;

        void allocateMemoryForBoneUniformGlobal();
        void deallocateMemoryForBoneUniformGlobal();
        void render();
        void formBoneMatrices(int objectID);
        void formTextureUniform(int meshID);
        void setBuffersAndDraw(int meshID, int objectID);
        void traverceBoneGraph( aiNode* node, glm::mat4* parentTransform, glm::mat4** boneUniform);
        glm::mat4 aiMatrix4x4toMat4 (aiMatrix4x4 aiMatrix);
        int findBoneInAnimation (string name);
        glm::vec3 findPositionVectorForCurrentTime(aiNodeAnim* channel);
        glm::quat findRoatationQuatForCurrentTime(aiNodeAnim* channel);
        glm::vec3 findScaleVectorForCurrentTime(aiNodeAnim* channel);
        void printMatrix(aiMatrix4x4 aiMatrix);
        void printMatrix(glm::mat4 aiMatrix);
        void pushObjectRenderData(a3dObjectRenderData* objectRenderData);
        void shuffleObjects();

    protected:

    private:
};

#endif // ARENDER3DOBJECTS_H
