/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Material source            */
/*                                        */
/*                                        */

#include "frost_material.h"

#if FRONT_END_OGRE
    #include "impl/ogre/ogre_material.h"
#elif FRONT_END_SFML
    #include "impl/sfml/sfml_material.h"
#else
    #error "No implementation defined for Font !"
#endif

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

Material::Material()
{
    pImpl_ = s_refptr<MaterialImpl>(new MaterialImpl());
}

Material::Material( s_refptr<MaterialImpl> pImpl )
{
    pImpl_ = pImpl;
}

Material::~Material()
{
}

Material::Material( const Material& mMat ) : pImpl_(mMat.pImpl_)
{
}

s_float Material::GetWidth() const
{
    return pImpl_->GetWidth();
}

s_float Material::GetHeight() const
{
    return pImpl_->GetHeight();
}

s_wptr<MaterialImpl> Material::GetImpl() const
{
    return pImpl_;
}

