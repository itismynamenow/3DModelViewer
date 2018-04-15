#ifndef TEST_H
#define TEST_H

#include <unordered_map>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class test
{
    public:
        test();
        virtual ~test();
        const aiScene* mScene;
        void loadMesh(std::string fileName);

    protected:

    private:
};

#endif // TEST_H
