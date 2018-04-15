#ifndef AMESH_H
#define AMESH_H

#include <string>

#include <glm/glm.hpp>


class aMesh
{
    public:
        aMesh();
        virtual ~aMesh();

        glm::vec3                   *mVertices;
        glm::vec3                   *mNormals;
        glm::vec3                   *mTangents;
        glm::vec3                   *mTextureCoords;

        int                         mNumVertices;
        int                         mNumOfUVComponents;

        std::string                 mName;

        bool                        mHasNormals;
        bool                        mHasTangents;
        bool                        mHasTextureCoordinates;

    protected:

    private:
};

#endif // AMESH_H
