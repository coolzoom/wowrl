/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Material source            */
/*                                        */
/*                                        */

#include "impl/sfml/sfml_material.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

MaterialImpl::MaterialImpl()
{
}

MaterialImpl::MaterialImpl( s_ptr<sf::Image> pImage )
{
    pImage_ = pImage;
}

MaterialImpl::~MaterialImpl()
{
}

s_float MaterialImpl::GetWidth() const
{
    return pImage_->GetWidth();
}

s_float MaterialImpl::GetHeight() const
{
    return pImage_->GetHeight();
}

s_ptr<sf::Image> MaterialImpl::GetImage() const
{
    return pImage_;
}
