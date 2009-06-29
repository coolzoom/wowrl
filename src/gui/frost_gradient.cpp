/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Gradient source             */
/*                                        */
/*                                        */

#include "gui/frost_gradient.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const Gradient Gradient::EMPTY_GRADIENT = Gradient();

Gradient::Gradient() : bIsEmpty_(true)
{
}


Gradient::Gradient( GradientOrientation mOrientation, const Color& mMinColor, const Color& mMaxColor ) :
    mOrientation_(mOrientation), mMinColor_(mMinColor), mMaxColor_(mMaxColor)
{
}

const Color& Gradient::GetMinColor() const
{
    return mMinColor_;
}

const Color& Gradient::GetMaxColor() const
{
    return mMaxColor_;
}

GradientOrientation Gradient::GetOrientation() const
{
    return mOrientation_;
}

const s_bool& Gradient::IsEmpty() const
{
    return bIsEmpty_;
}


