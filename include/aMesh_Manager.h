#ifndef AMESH_MANAGER_H
#define AMESH_MANAGER_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


class aMesh_Manager
{
    public:
        aMesh_Manager();
        virtual ~aMesh_Manager();
        aiScene* m_Main_Scene;

    protected:

    private:
};

#endif // AMESH_MANAGER_H
