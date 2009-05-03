/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GFX source                */
/*                                        */
/*                                        */


#include "frost_gfx.h"
#include "frost_material.h"

#include <OgreColourValue.h>

using namespace std;

namespace Frost
{
    void GFX::FrostToOgrePosition( s_float &fX, s_float &fY, s_float &fZ )
    {
        s_float fTemp = fX;
        fX = fY;
        fY = fZ;
        fZ = fTemp;
    }

    void GFX::OgreToFrostPosition( s_float &fX, s_float &fY, s_float &fZ )
    {
        s_float fTemp = fX;
        fX = fZ;
        fZ = fY;
        fY = fTemp;
    }

    Ogre::ColourValue GFX::FrostToOgreColor( const Color& mColor )
    {
        Ogre::ColourValue mOgreColor;
        mOgreColor.setAsARGB(mColor.Get().Get());
        return mOgreColor;
    }
}
