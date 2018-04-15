#ifndef MEXTRANODEDATA_H
#define MEXTRANODEDATA_H


class mExtraNodeData
{
    public:
        mExtraNodeData();
        virtual ~mExtraNodeData();
        int                             *mAnimationIndices,
                                        *mAnimStartTime;
        bool                            *mHasAnim,
                                        *mAnimIsPlaing;

    protected:

    private:
};

#endif // MEXTRANODEDATA_H
