/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Material header            */
/*                                        */
/*                                        */

#ifndef FROST_GUI_MATERIALIMPL_H
#define FROST_GUI_MATERIALIMPL_H

#include <frost_utils.h>

namespace sf
{
    class Image;
}

namespace Frost
{
namespace GUI
{
    class MaterialImpl
    {
    public :

        MaterialImpl();

        MaterialImpl(s_ptr<sf::Image> pImage);

        ~MaterialImpl();

        s_float GetWidth() const;

        s_float GetHeight() const;

        s_ptr<sf::Image> GetImage() const;

    private :

        s_ptr<sf::Image> pImage_;
    };
}
}

#endif
