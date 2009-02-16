/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Gradient source             */
/*                                        */
/*                                        */

#include "gui/frost_gui_gradient.h"

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

const s_bool& Gradient::IsEmpty() const
{
    return bIsEmpty_;
}


