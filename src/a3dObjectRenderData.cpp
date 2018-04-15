#include "a3dObjectRenderData.h"

a3dObjectRenderData::a3dObjectRenderData()
{
    mObjectTransform=glm::mat4();
    mHasAnimationPlaying=true;
    mAnimationTimeOffset=0;
}

a3dObjectRenderData::~a3dObjectRenderData()
{
    //dtor
}

void a3dObjectRenderData::setObject(a3dObjectData* object)
{
    mObject3d=object;
}

void a3dObjectRenderData::update()
{
    mAnimationTime=fmod((float)Time.asMilliseconds()/1000.0+mAnimationTimeOffset,(float)mObject3d->mScene->mAnimations[0]->mDuration);
}
