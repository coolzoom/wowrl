/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Gradient source             */
/*                                        */
/*                                        */

#include "frost_gradient.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const Gradient Gradient::NONE = Gradient();

Gradient::Gradient() : bIsEmpty_(true)
{
}


Gradient::Gradient( Orientation mOrientation, const Color& mMinColor, const Color& mMaxColor ) :
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

Gradient::Orientation Gradient::GetOrientation() const
{
    return mOrientation_;
}

const s_bool& Gradient::IsEmpty() const
{
    return bIsEmpty_;
}


