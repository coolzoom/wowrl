/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Material header            */
/*                                        */
/*                                        */

#ifndef FROST_GUI_MATERIALIMPL_H
#define FROST_GUI_MATERIALIMPL_H

#include <frost_utils.h>

namespace Ogre
{
    class Material;
}

namespace Frost
{
namespace GUI
{
    class RenderTarget;

    class MaterialImpl
    {
    public :

        MaterialImpl();

        MaterialImpl(s_ptr<Ogre::Material> pMat);

        MaterialImpl(s_ptr<Ogre::Material> pMat, const s_float& fWidth, const s_float& fHeight);

        MaterialImpl(const s_str& sFileName);

        MaterialImpl(const Color& mColor);

        MaterialImpl(s_ptr<RenderTarget> pRenderTarget);

        ~MaterialImpl();

        s_float GetWidth() const;

        s_float GetHeight() const;

        s_ptr<Ogre::Material> GetOgreMaterial() const;

    private :

        void GetDimensions_();
        s_uint GetNewID_();

        s_ptr<Ogre::Material> pMat_;
        s_float fWidth_, fHeight_;
    };
}
}

#endif
