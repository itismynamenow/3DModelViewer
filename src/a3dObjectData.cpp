#include "a3dObjectData.h"

a3dObjectData::a3dObjectData()
{
    mHasTextures=false;
}

a3dObjectData::~a3dObjectData()
{
    //dtor
}

void a3dObjectData::load3dObject(std::string fileName)
{
    mName=fileName;
    mScene = Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    if (!mScene)
    {
        std::cout<<"Error parsing "<< fileName.c_str()<< Importer.GetErrorString()<<std::endl;
    }
    mNumMeshes=mScene->mNumMeshes;
    analyzeScene();

    if(mHasTextureCoords)
    {
//        std::cout<<"Has TC"<<std::endl;
        loadTextures();
    }

    if(mHasBones)
    {
        //        std::cout<<"Has Bones"<<std::endl;

        formBoneMaps();
        formWeightsAndBonesIdArrays();
    }
    formBuffers();
    printMeshData();
}

void a3dObjectData::formBoneMaps()
{
    mBoneMap= new std::unordered_map<std::string,int> [mScene->mNumMeshes];
    for(int i=0;i<mScene->mNumMeshes;i++)
    {
        for(int j=0;j<mScene->mMeshes[i]->mNumBones;j++)
        {
            mBoneMap[i][(std::string) mScene->mMeshes[i]->mBones[j]->mName.data]=j;
            mBoneNameToMeshIdMap [(std::string) mScene->mMeshes[i]->mBones[j]->mName.data]=i;
        }
    }

    for(int i=0;i<mScene->mAnimations[0]->mNumChannels;i++)
    {
        mBoneNameToAnimIdMap[(std::string)mScene->mAnimations[0]->mChannels[i]->mNodeName.data]=i;
    }


}

void a3dObjectData::formWeightsAndBonesIdArrays()
{
        int numMeshes=mScene->mNumMeshes;

        int *offsetVertexArray[numMeshes];

        mWeightsArray=new float*[numMeshes];
        mIndexArray=new int*[numMeshes];

        for(int i=0;i<numMeshes;i++)
        {
            offsetVertexArray[i]=new int[mScene->mMeshes[i]->mNumVertices];
            memset(offsetVertexArray[i], 0, mScene->mMeshes[i]->mNumVertices*sizeof(*offsetVertexArray[i]));

            mWeightsArray[i]=new float[mScene->mMeshes[i]->mNumVertices*4];
            memset(mWeightsArray[i], 0, mScene->mMeshes[i]->mNumVertices*sizeof(*mWeightsArray[i])*4);

            mIndexArray[i]=new int[mScene->mMeshes[i]->mNumVertices*4];
            memset(mIndexArray[i], -1, mScene->mMeshes[i]->mNumVertices*sizeof(*mIndexArray[i])*4);
        }

        for(int i=0;i<numMeshes;i++)
        {
            for(int j=0;j<mScene->mMeshes[i]->mNumBones;j++)
            {
                for(int k=0;k<mScene->mMeshes[i]->mBones[j]->mNumWeights;k++)
                {
                    if(offsetVertexArray[i][mScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId]<4)
                    {
                        mWeightsArray[i][mScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId*4+
                        offsetVertexArray[i][mScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId]]=
                            mScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
                        mIndexArray[i][mScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId*4+
                            offsetVertexArray[i][mScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId]]=j;
                        offsetVertexArray[i][mScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId]++;
                    }
                }
            }
        }
}

void a3dObjectData::formBuffers()
{
    mVBO = new GLuint [mNumMeshes];
    if(mHasBones)
    {
        mIndexBuffer = new GLuint [mNumMeshes];
        mWeightsBuffer = new GLuint [mNumMeshes];
    }
    if(mHasTextureCoords)
    {
         mTextureUVBuffer = new GLuint [mNumMeshes];
    }

    for(int i=0;i<mNumMeshes;i++)
    {
        glGenBuffers(1,&mVBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER,mVBO[i]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*mScene->mMeshes[i]->mNumVertices,mScene->mMeshes[i]->mVertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        if(mHasBonesArray[i])
        {
            glGenBuffers(1,&mIndexBuffer[i]);
            glBindBuffer(GL_ARRAY_BUFFER, mIndexBuffer[i]);
            glBufferData(GL_ARRAY_BUFFER,sizeof(GLint)*4*mScene->mMeshes[i]->mNumVertices,mIndexArray[i],GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);

            glGenBuffers(1,&mWeightsBuffer[i]);
            glBindBuffer(GL_ARRAY_BUFFER,mWeightsBuffer[i]);
            glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*4*mScene->mMeshes[i]->mNumVertices,mWeightsArray[i],GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }

        if(mHasTextureCoordsArray[i])
        {
//            std::cout<<mName<<std::endl;
//            std::cout<<i<<std::endl;
            glGenBuffers(1,&mTextureUVBuffer[i]);
            glBindBuffer(GL_ARRAY_BUFFER,mTextureUVBuffer[i]);
            glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*mScene->mMeshes[i]->mNumVertices,mScene->mMeshes[i]->mTextureCoords[0],GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
    }
}

void a3dObjectData::analyzeScene()
{

    mHasAnimations = mScene->HasAnimations();

    mHasTextureCoords=false;
    mHasNormals=false;
    mHasTangentsAndBitangents=false;
    mHasBones=false;

    mHasTextureCoordsArray = new bool [mNumMeshes];
    mHasNormalsArray = new bool [mNumMeshes];
    mHasTangentsAndBitangentsArray = new bool [mNumMeshes];
    mHasBonesArray = new bool [mNumMeshes];
    mNumUVChannels = new int [mNumMeshes];
    mMaterialIndecies = new int [mNumMeshes];

    for(int i=0;i<mNumMeshes;i++)
    {
        mHasTextureCoordsArray[i]=mScene->mMeshes[i]->HasTextureCoords(0);
        mHasNormalsArray[i]=mScene->mMeshes[i]->HasNormals();
        mHasTangentsAndBitangentsArray[i]=mScene->mMeshes[i]->HasTangentsAndBitangents();
        mHasBonesArray[i]=mScene->mMeshes[i]->HasBones();

        if(mScene->mMeshes[i]->HasTextureCoords(0))
            mHasTextureCoords=true;

        if(mScene->mMeshes[i]->HasNormals())
            mHasNormals=true;

        if(mScene->mMeshes[i]->HasTangentsAndBitangents())
            mHasTangentsAndBitangents=true;

        if(mScene->mMeshes[i]->HasBones())
            mHasBones=true;

        mMaterialIndecies[i]=mScene->mMeshes[i]->mMaterialIndex;
        mNumUVChannels[i]=mScene->mMeshes[i]->GetNumUVChannels();
    }

}

void a3dObjectData::printMeshData()
{

//    std::cout<<" embeded text count"<<mScene->mNumTextures<<std::endl;
//    for(int i=0;i<mScene->mNumMaterials;i++)
//    {
//           const aiMaterial* material = mScene->mMaterials[i];
//            aiString texturePath;
//            std::string path;
//            unsigned int numTextures= material->GetTextureCount(aiTextureType_DIFFUSE);   // always 0
//
////            std::cout<<" text count dif "<<material->GetTextureCount(aiTextureType_DIFFUSE) <<std::endl;
////            std::cout<<" text count spec "<<material->GetTextureCount(aiTextureType_SPECULAR) <<std::endl;
////            std::cout<<" text count amb "<<material->GetTextureCount(aiTextureType_AMBIENT) <<std::endl;
////            std::cout<<" text count unkn "<<material->GetTextureCount(aiTextureType_UNKNOWN) <<std::endl;
//
//            if ( material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
//            {
////                std::cout<<texturePath.data<<std::endl;
//                std::stringstream ss;
//                ss<<(std::string)texturePath.data;
//
//                while (getline(ss, path, (char)47))
//                    {
//                    }
//                std::stringstream ss2;
//                ss2<<(std::string)path;
//                while (getline(ss2, path, (char)92))
//                    {
//                    }
//                std::cout<<path<<std::endl;
//            }
//    }
}

 void a3dObjectData::loadTextures()
 {
    mImageArray = new sf::Image [mNumMeshes];
    mTextureArray = new GLuint[mNumMeshes];
    mTextureNames = new std::string [mScene->mNumMaterials];
    mTextureCorrespondentToMesheArray = new GLuint [mNumMeshes];
//        std::cout<<"numMaterials "<<mScene->mNumMaterials<<std::endl;

    for(int i=0;i<mScene->mNumMaterials;i++)
    {
        const aiMaterial* material = mScene->mMaterials[i];
        aiString texturePath;
        std::string path;
        unsigned int numTextures= material->GetTextureCount(aiTextureType_DIFFUSE);

//        std::cout<<"numTextures DIFF "<<material->GetTextureCount(aiTextureType_DIFFUSE)<<std::endl;
//        std::cout<<"numTextures SPEC "<<material->GetTextureCount(aiTextureType_SPECULAR)<<std::endl;
//        std::cout<<"numTextures AMB "<<material->GetTextureCount(aiTextureType_AMBIENT)<<std::endl;
//        std::cout<<"numTextures NORM "<<material->GetTextureCount(aiTextureType_NORMALS)<<std::endl;
//        std::cout<<"numTextures UNKN "<<material->GetTextureCount(aiTextureType_UNKNOWN)<<std::endl;
//        std::cout<<"numTextures EMISSIVE "<<material->GetTextureCount(aiTextureType_EMISSIVE)<<std::endl;
//        std::cout<<"numTextures DISP "<<material->GetTextureCount(aiTextureType_DISPLACEMENT)<<std::endl;
//        std::cout<<"numTextures NONE "<<material->GetTextureCount(aiTextureType_NONE)<<std::endl;
//        std::cout<<"numTextures NONE "<<material->GetTextureCount(aiTextureType_)<<std::endl;

        if ( material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
        {
            std::stringstream ss;
            ss<<(std::string)texturePath.data;
            while (getline(ss, path, (char)47)){}

            std::stringstream ss2;
            ss2<<(std::string)path;
            while (getline(ss2, path, (char)92)){}

//            path="m4tex.png";

            mTextureNames[i] = path;
//        std::cout<<path<<std::endl;
            if(!mImageArray[i].loadFromFile(path))
            {
                std::cout<<"Failed to load texture "<<path<<std::endl;
            }

            else
            {
                mHasTextures=true;
                glGenTextures( 1, &mTextureArray[i] );
                glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureArray[i]);
                glTexStorage3D( GL_TEXTURE_2D_ARRAY,
                                1,                                                                      //No mipmaps as textures are 1x1
                                GL_RGBA8,                                                               //Internal format
                                mImageArray[i].getSize().x, mImageArray[i].getSize().y,                 //width,height
                                1);
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                                0,                     //Mipmap number
                                0,0,0,                 //xoffset, yoffset, zoffset
                                mImageArray[i].getSize().x, mImageArray[i].getSize().y,1,                 //width, height, depth
                                GL_RGBA,                //format
                                GL_UNSIGNED_BYTE,      //type
                                mImageArray[i].getPixelsPtr());                //pointer to data
                glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
//                std::cout<<mImageArray[i].getSize().x<<" "<< mImageArray[i].getSize().y<<std::endl;
            }
        }
    }
    for(int i=0;i<mScene->mNumMeshes;i++)
    {
        for(int j=0;j<mScene->mNumMaterials;j++)
        {
            if(mScene->mMeshes[i]->mMaterialIndex==j)
            {
                mTextureCorrespondentToMesheArray[i]=mTextureArray[j];
            }
        }
    }
 }
