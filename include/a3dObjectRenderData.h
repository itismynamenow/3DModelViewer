#ifndef A3DOBJECTRENDERDATA_H
#define A3DOBJECTRENDERDATA_H
#include <glm/glm.hpp>

#include "a3dObjectData.h"

extern sf::Time Time;

class a3dObjectRenderData
{
    public:
        a3dObjectRenderData();
        virtual ~a3dObjectRenderData();

        a3dObjectData*                      mObject3d;

        glm::mat4                           mObjectTransform;

        bool                                mHasAnimationPlaying;

        int                                 mObject3dID;
        int                                 mAnimationId;
        float                               mAnimationTime;
        float                               mAnimationTimeOffset;

        void setObject(a3dObjectData* object);
        void update();

    protected:

    private:
};

#endif // A3DOBJECTRENDERDATA_H
