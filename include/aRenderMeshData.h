#ifndef ARENDERMESHDATA_H
#define ARENDERMESHDATA_H
#include <unordered_map>

class aRenderMeshData
{
    public:
        aRenderMeshData();
        virtual ~aRenderMeshData();
        int mMeshId;
        float mAnimTime;
        //TODO remove bone map
        std::unordered_map<std::string,int> boneMap;

    protected:

    private:
};

#endif // ARENDERMESHDATA_H
