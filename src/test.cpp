#include "test.h"

test::test()
{
    //ctor
}

test::~test()
{
    //dtor
}

void test::loadMesh(std::string fileName)
{
    Assimp::Importer Importer;
    mScene = Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (!mScene)
    {
        std::cout<<"Error parsing "<< fileName.c_str()<< Importer.GetErrorString()<<std::endl;
    }
    std::cout<<mScene->mMeshes[0]->mVertices[0].x<<std::endl;
    std::cout<<mScene->mMeshes[0]->mTextureCoords[0][0].y<<std::endl;
}
