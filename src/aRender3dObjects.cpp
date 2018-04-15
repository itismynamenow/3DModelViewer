#include "aRender3dObjects.h"

aRender3dObjects::aRender3dObjects()
{

}

aRender3dObjects::~aRender3dObjects()
{
    //dtor
}

void aRender3dObjects::render() //works
{
    static double count=0;
    static double sum=0;
//    t1 = Clock.getElapsedTime().asMicroseconds();

    allocateMemoryForBoneUniformGlobal();

    for(int h=0;h<mShuffledObjects.size();h++)
    {
        a3dObjectRenderData* objectData = mShuffledObjects[h][0];
        mObjectData=objectData;

        if(mObjectData->mHasAnimationPlaying&&mObjectData->mObject3d->mHasBones&&mObjectData->mObject3d->mScene->mNumAnimations>0)//20000mrs
                {
//                    t1 = Clock.getElapsedTime().asMicroseconds();
                    formBoneMatrices(h);
//                    t2 = Clock.getElapsedTime().asMicroseconds();
//                    std::cout<<"Render (microSec) "<<t2-t1<<std::endl;
                }


        for(int i=0;i<objectData->mObject3d->mNumMeshes;i++)//700mrs
        {
            if(i!=-1)
            {

                mObjectData=objectData;

                if(mObjectData->mObject3d->mHasTextures&&mObjectData->mObject3d->mHasTextureCoordsArray[i])
                {
                    formTextureUniform(i);//2mrs
                }

                mObjectData=objectData;

                setBuffersAndDraw(i,h);// 2000mrs

            }
        }
//                t2 = Clock.getElapsedTime().asMicroseconds();
//
//        std::cout<<"Render (microSec) "<<t2-t1<<std::endl;
    }






    deallocateMemoryForBoneUniformGlobal();

//    for(int h=0;h<shaffledObjects.size();h++)
//    {
//       delete &shaffledObjects[h];
//    }

    mShuffledObjects.clear();
    mObjectRenderDataVector.clear();
//    t2 = Clock.getElapsedTime().asMicroseconds();
//    sum+=t2-t1;
//    count++;
//    std::cout<<" Form text u "<<sum/count<<std::endl;
}

void aRender3dObjects::formBoneMatrices(int objectID) //works
{
    for(int h=0;h<mShuffledObjects[objectID].size();h++)
    {
        glm::mat4 identityMatrix =glm::mat4();
        mObjectData=mShuffledObjects[objectID][h];

//        t1 = Clock.getElapsedTime().asMicroseconds();
        traverceBoneGraph(mObjectData->mObject3d->mScene->mRootNode,&identityMatrix, mBoneUniformGlobal[objectID][h]);
//         t2 = Clock.getElapsedTime().asMicroseconds();
//         std::cout<<"entire traverce "<<t2-t1<<std::endl;//170-120MRS
    }
}

void aRender3dObjects::formTextureUniform(int meshID)
{

}

void aRender3dObjects::setBuffersAndDraw(int meshID, int objectID)
{
//        std::cout<<"Forming buffer "<<std::endl;
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mObjectData->mObject3d->mVBO[meshID]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

//    if(mObjectData->mHasAnimationPlaying&&mObjectData->mObject3d->mHasBonesArray[meshID]&&mObjectData->mObject3d->mHasAnimations)
    if(mObjectData->mHasAnimationPlaying&&mObjectData->mObject3d->mHasBonesArray[meshID]&&mObjectData->mObject3d->mHasAnimations)
    {
//        std::cout<<"Forming bone buffer "<<std::endl;
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mObjectData->mObject3d->mWeightsBuffer[meshID]);
        glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, mObjectData->mObject3d->mIndexBuffer[meshID]);
        glVertexAttribIPointer(2,4,GL_INT,0,0);
    }

    if(mObjectData->mObject3d->mHasTextures&&mObjectData->mObject3d->mHasTextureCoordsArray[meshID])
    {
//        std::cout<<"Forming text buffer "<<std::endl;
//        std::cout<<meshID<<std::endl;
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, mObjectData->mObject3d->mTextureUVBuffer[meshID]);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,0);
        glActiveTexture(GL_TEXTURE0);
        int materialIndex = mObjectData->mObject3d->mScene->mMeshes[meshID]->mMaterialIndex;
        glBindTexture(GL_TEXTURE_2D_ARRAY, mObjectData->mObject3d->mTextureArray[materialIndex]);

        glUniform1i(Shader.uniformMap["textureSampler"], 0);
    }

    for(int i=0;i<mShuffledObjects[objectID].size();i++)
    {
        mObjectData=mShuffledObjects[objectID][i];
        glm::mat4 productOfCameraAndTransformMatrices=Camera.CameraMatrix*mObjectData->mObjectTransform;

        glUniformMatrix4fv(Shader.uniformMap["transform"],1,GL_FALSE,&productOfCameraAndTransformMatrices[0][0]);
        glUniform1i(Shader.uniformMap["displayTexture"],bool(mObjectData->mObject3d->mHasTextures&&mObjectData->mObject3d->mHasTextureCoordsArray[meshID]));
        glUniform1i(Shader.uniformMap["playAnimation"],bool(mObjectData->mHasAnimationPlaying&&mObjectData->mObject3d->mHasBonesArray[meshID]&&mObjectData->mObject3d->mHasAnimations));
        glUniformMatrix4fv(Shader.uniformMap["bones"],mObjectData->mObject3d->mScene->mMeshes[meshID]->mNumBones,GL_FALSE,glm::value_ptr(mBoneUniformGlobal[objectID][i][meshID][0]));

        glDrawArrays(GL_TRIANGLES, 0, mObjectData->mObject3d->mScene->mMeshes[meshID]->mNumVertices);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void aRender3dObjects::traverceBoneGraph(aiNode* node, glm::mat4* parentTransform, glm::mat4** boneUniform)//goes to 4mrs
{
    static double count=0;
    static double sum=0;
//    t1 = Clock.getElapsedTime().asMicroseconds();
    //for animation we first take matrix from current aiNode.
    glm::mat4 nodeTransform=aiMatrix4x4toMat4(node->mTransformation);
//    //if we encounter node that has correspondent bone we replace above matrix with matrix made on basis of transform from animation channel with name of this bone at particular time


    int findBoneInAnimationValue = findBoneInAnimation((string)node->mName.data); //2.5mas
    int boneID[mObjectData->mObject3d->mNumMeshes];

    for(int i=0;i<mObjectData->mObject3d->mNumMeshes;i++)//5 mas
    {
        boneID[i]=-1;
        std::unordered_map<std::string,int>::const_iterator got = mObjectData->mObject3d->mBoneMap[i].find(node->mName.data);
        if(got!= mObjectData->mObject3d->mBoneMap[i].end())
        {
            boneID[i]=got->second;
        }
    }

    if(findBoneInAnimationValue!=-1) //6mrs

    {
        glm::mat4 position = glm::translate(findPositionVectorForCurrentTime(mObjectData->mObject3d->mScene->mAnimations[0]->mChannels[findBoneInAnimationValue]));
        glm::mat4 rotation = glm::toMat4(findRoatationQuatForCurrentTime(mObjectData->mObject3d->mScene->mAnimations[0]->mChannels[findBoneInAnimationValue])); //3mas
        glm::mat4 scale = glm::scale(findScaleVectorForCurrentTime(mObjectData->mObject3d->mScene->mAnimations[0]->mChannels[findBoneInAnimationValue]));

//            t2 = Clock.getElapsedTime().asMicroseconds();
//    sum+=t2-t1;
//    count+=1;
//    std::cout<<"single find bone "<<sum/count<<std::endl;

        nodeTransform=position*rotation*scale;
    }
//t1 = Clock.getElapsedTime().asMicroseconds();

//    //now we form global transform that is used as parent transform for child node
    glm::mat4 globalTransform = *parentTransform * nodeTransform;
//    //here we check if current node has bone from mesh that we drawing now

    for(int i=0;i<mObjectData->mObject3d->mNumMeshes;i++)
    {
        if(boneID[i]!=-1)
        {
//            glm::mat4 offset=aiMatrix4x4toMat4(mObjectData->mObject3d->mScene->mMeshes[i]->mBones[boneID[i]]->mOffsetMatrix);
            boneUniform [i][boneID[i]] = globalTransform*aiMatrix4x4toMat4(mObjectData->mObject3d->mScene->mMeshes[i]->mBones[boneID[i]]->mOffsetMatrix);
        }
    }
//
//    t2 = Clock.getElapsedTime().asMicroseconds();
//    sum+=t2-t1;
//    count+=1;
//    std::cout<<"single traverce iteration "<<sum/count<<std::endl;//15//13//17
    for(int i=0;i<node->mNumChildren;i++)
    {
       if(node->mChildren[i]!=NULL)
       {
           traverceBoneGraph(node->mChildren[i],&globalTransform, boneUniform);
       }
    }

}

glm::mat4 aRender3dObjects::aiMatrix4x4toMat4(aiMatrix4x4 aiMatrix)
{
    glm::mat4 glmMat4;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            glmMat4[j][i]=aiMatrix[i][j];
        }
    }
    return glmMat4;
}

int aRender3dObjects::findBoneInAnimation(string name)//4.8mrs - 2.5mrs after 1 cycle
{
//    static double count=0;
//    static double sum=0;
//    t1 = Clock.getElapsedTime().asMicroseconds();
    std::unordered_map<std::string,int>::const_iterator got = mObjectData->mObject3d->mBoneNameToAnimIdMap.find(name);
    if(got!=mObjectData->mObject3d->mBoneNameToAnimIdMap.end())
    {
//        t2 = Clock.getElapsedTime().asMicroseconds();
//        sum+=t2-t1;
//        count++;
//        std::cout<<"Find S "<<sum/count<<std::endl;
        return got->second;
    }
    else
    {
//        t2 = Clock.getElapsedTime().asMicroseconds();
//        sum+=t2-t1;
//        count++;
//        std::cout<<"Find F "<<sum/count<<std::endl;
        return -1;
    }
//    std::cout<<"single findboneinamin iteration Fail "<<t2-t1<<std::endl;

}

glm::vec3 aRender3dObjects::findPositionVectorForCurrentTime(aiNodeAnim* channel)
{
    aiVectorKey* tmp = std::upper_bound(&channel->mPositionKeys[0],
                                  &channel->mPositionKeys[channel->mNumPositionKeys],
                                  mObjectData->mAnimationTime,
                                    [](float val, aiVectorKey& a)
                                     { return(float)a.mTime > val; });
    return glm::vec3(tmp->mValue.x,tmp->mValue.y,tmp->mValue.z);
}

glm::quat aRender3dObjects::findRoatationQuatForCurrentTime(aiNodeAnim* channel)
{
        aiQuatKey* tmp = std::upper_bound(&channel->mRotationKeys[0],
                                          &channel->mRotationKeys[channel->mNumRotationKeys],
                                          mObjectData->mAnimationTime,
                                            [](float val, aiQuatKey& a)
                                             { return(float)a.mTime > val; });

        return glm::normalize(glm::quat(tmp->mValue.w,
                                        tmp->mValue.x,
                                        tmp->mValue.y,
                                        tmp->mValue.z));


}

glm::vec3 aRender3dObjects::findScaleVectorForCurrentTime(aiNodeAnim* channel)
{
    aiVectorKey* tmp = std::upper_bound(&channel->mScalingKeys[0],
                                  &channel->mScalingKeys[channel->mNumScalingKeys],
                                  mObjectData->mAnimationTime,
                                    [](float val, aiVectorKey& a)
                                     { return(float)a.mTime > val; });
    return glm::vec3(tmp->mValue.x,tmp->mValue.y,tmp->mValue.z);

}

void aRender3dObjects::printMatrix(aiMatrix4x4 aiMatrix)
{
    static int count=0;
    cout<<"-------------------------------"<<endl;
    cout<<count<<endl;
    count++;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            cout<<aiMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
}

void aRender3dObjects::printMatrix(glm::mat4 aiMatrix)
{
    static int count=0;
    cout<<"-------------------------------"<<endl;
    cout<<count<<endl;
    count++;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            cout<<aiMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
}

void aRender3dObjects::pushObjectRenderData(a3dObjectRenderData* objectRenderData)
{
    objectRenderData->update();
    mObjectRenderDataVector.push_back(objectRenderData);
}

void aRender3dObjects::shuffleObjects()
{
    t1 = Clock.getElapsedTime().asMicroseconds();
    std::unordered_map<std::string,int>  meshToIDMap;
    for(int i=0;i<mObjectRenderDataVector.size();i++)
    {
        if(meshToIDMap.find(mObjectRenderDataVector[i]->mObject3d->mName)!=meshToIDMap.end())
        {
            int ID = meshToIDMap[mObjectRenderDataVector[i]->mObject3d->mName];
            mShuffledObjects[ID].push_back(mObjectRenderDataVector[i]);
        }
        else
        {
            meshToIDMap[mObjectRenderDataVector[i]->mObject3d->mName]=mShuffledObjects.size();
            vector<a3dObjectRenderData*> tmp = *new vector<a3dObjectRenderData*>;
            tmp.push_back(mObjectRenderDataVector[i]);
            mShuffledObjects.push_back(tmp);
        }
    }

    t2 = Clock.getElapsedTime().asMicroseconds();

//    std::cout<<"shuffle time (microSec) "<<t2-t1<<std::endl;
//    std::cout<<"num Obj "<<shaffledObjects.size()<<std::endl;

//    for(int i=0;i<shaffledObjects.size();i++)
//    {
//        std::cout<<"--------------------------"<<std::endl;
//        for(int j=0; j<shaffledObjects[i].size();j++)
//        {
//            std::cout<<shaffledObjects[i][j]->mObject3d->mName<<std::endl;
//        }
//    }
}

void aRender3dObjects::allocateMemoryForBoneUniformGlobal()
{
    mBoneUniformGlobal=new glm::mat4*** [mShuffledObjects.size()];//num of groups of same obj
    for(int i=0;i<mShuffledObjects.size();i++)
    {
        mBoneUniformGlobal[i]= new glm::mat4** [mShuffledObjects[i].size()]; //num of object in group
        {
            for(int j=0;j<mShuffledObjects[i].size();j++)
            {
                mBoneUniformGlobal[i][j]= new glm::mat4* [mShuffledObjects[i][0]->mObject3d->mNumMeshes];//num meshes in object
                for(int k=0;k<mShuffledObjects[i][0]->mObject3d->mNumMeshes;k++)
                {
                    mBoneUniformGlobal[i][j][k]= new glm::mat4 [mShuffledObjects[i][0]->mObject3d->mScene->mMeshes[k]->mNumBones]; //num bones in mesh
                }
            }
        }
    }
}

void aRender3dObjects::deallocateMemoryForBoneUniformGlobal()
{
    for(int i=0;i<mShuffledObjects.size();i++)
    {
        {
            for(int j=0;j<mShuffledObjects[i].size();j++)
            {
                for(int k=0;k<mShuffledObjects[i][0]->mObject3d->mNumMeshes;k++)
                {
                    delete [] mBoneUniformGlobal[i][j][k];
                }
                delete []mBoneUniformGlobal[i][j];
            }
            delete[] mBoneUniformGlobal[i];
        }
        delete[] mBoneUniformGlobal;
    }
}
