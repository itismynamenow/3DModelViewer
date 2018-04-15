#ifndef AEXTRANODEDATA_H
#define AEXTRANODEDATA_H


class aExtraNodeData
{
    public:
        aExtraNodeData();
        virtual ~aExtraNodeData();
        int                             *mAnimationIndices,
                                        *mAnimStartTime;
        bool                            *mHasAnim,
                                        *mAnimIsPlaing;

    protected:

    private:
};

#endif // AEXTRANODEDATA_H
