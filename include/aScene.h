#ifndef ASCENE_H
#define ASCENE_H

#include "aMesh.h"
#include "aNode.h"


class aScene
{
    public:
        aScene();
        virtual ~aScene();
        aMesh                       **mMeshes;
        aNode                       mRootNode;

        int                         mNumMeshes;


    protected:

    private:
};

#endif // ASCENE_H
