#ifndef ANODE_H
#define ANODE_H

#include <string>

#include <glm/glm.hpp>

class aNode
{
    public:
        aNode();
        virtual ~aNode();

        aNode                       **mChildren;
        aNode                       *mParent;

        int                         mNumOfChildren;
        int                         mNumOfMeshes;

        glm::mat4                   mTransformMatrix;

        std::string                 mName;

    protected:

    private:
};

#endif // ANODE_H
