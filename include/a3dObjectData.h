#ifndef A3DOBJECTDATA_H
#define A3DOBJECTDATA_H
#include <unordered_map>
#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "SFML/Graphics.hpp"

class a3dObjectData
{
    public:
        a3dObjectData();
        virtual ~a3dObjectData();
        const aiScene*                                  mScene;
        std::string                                     mName;
        std::string*                                    mTextureNames;
        std::unordered_map<std::string,int>*            mBoneMap;
        std::unordered_map<std::string,int>             mBoneNameToMeshIdMap;
        std::unordered_map<std::string,int>             mBoneNameToAnimIdMap;

        Assimp::Importer                                Importer;
        sf::Image*                                      mImageArray;

        bool                                            mHasAnimations;
        bool                                            mHasTextureCoords;
        bool                                            mHasTextures;
        bool                                            mHasNormals;
        bool                                            mHasTangentsAndBitangents;
        bool                                            mHasBones;

        bool*                                           mHasTextureCoordsArray;
        bool*                                           mHasNormalsArray;
        bool*                                           mHasTangentsAndBitangentsArray;
        bool*                                           mHasBonesArray;

        int                                             mNumMeshes;
        int*                                            mMaterialIndecies;
        int*                                            mNumUVChannels;
        int**                                           mIndexArray;
        float**                                         mWeightsArray;

        GLuint*                                         mVBO;
        GLuint*                                         mIndexBuffer;
        GLuint*                                         mWeightsBuffer;
        GLuint*                                         mTextureUVBuffer;
        GLuint*                                         mTextureArray;
        GLuint*                                         mTextureCorrespondentToMesheArray;

        int getAnimationNumber();
        int getMeshNumber();
        aiNode getRootNode();
        bool checkIfBonesPresent();

        void analyzeScene();
        void load3dObject(std::string fileName);
        void loadTextures();
        void formBoneMaps();
        void formWeightsAndBonesIdArrays();
        void formBuffers();
        void printMeshData();
        //TODO add shader type used;

    protected:

    private:
};

#endif // A3DOBJECTDATA_H
