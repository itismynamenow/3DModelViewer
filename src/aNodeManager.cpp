#include "aNodeManager.h"

aNodeManager::aNodeManager()
{
    //ctor
}

aNodeManager::~aNodeManager()
{
    //dtor
}

void aNodeManager::load(std::string Filename)
{
    Scene =  const_cast <aiScene*>(Importer.ReadFile(Filename.c_str(),
                                                     aiProcess_Triangulate |
                                                     aiProcess_GenSmoothNormals |
                                                     aiProcess_FlipUVs));
}

