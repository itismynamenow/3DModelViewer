#ifndef ANODEMANAGER_H
#define ANODEMANAGER_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class aNodeManager
{
    public:
        aNodeManager();
        virtual ~aNodeManager();
        Assimp::Importer Importer;
        aiScene* Scene;
        void load(std::string Filename);
        Scene->mAnimations->

    protected:

    private:
};

#endif // ANODEMANAGER_H
