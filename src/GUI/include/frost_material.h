/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Material header            */
/*                                        */
/*                                        */

#ifndef FROST_GUI_MATERIAL_H
#define FROST_GUI_MATERIAL_H

#include <frost_utils.h>

namespace Frost
{
namespace GUI
{
    class MaterialImpl;

    class Material
    {
    public :

        Material();

        Material(s_refptr<MaterialImpl> pImpl);

        Material(const Material& mMat);

        ~Material();

        s_float GetWidth() const;

        s_float GetHeight() const;

        s_wptr<MaterialImpl> GetImpl() const;

    private :

        s_refptr<MaterialImpl> pImpl_;
    };
}
}

#endif
