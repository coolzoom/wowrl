/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           RenderTarget source          */
/*                                        */
/*                                        */

#include "frost_rendertarget.h"

#if FRONT_END_OGRE
    #include "impl/ogre/ogre_rendertarget.h"
#elif FRONT_END_SFML
    #include "impl/sfml/sfml_rendertarget.h"
#else
    #error "No implementation defined for RenderTarget !"
#endif

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str RenderTarget::CLASS_NAME = "GUI::RenderTarget";

RenderTarget::RenderTarget( const s_uint& uiWidth, const s_uint& uiHeight )
{
    pImpl_ = s_refptr<RenderTargetImpl>(new RenderTargetImpl(uiWidth, uiHeight));
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Clear( const Color& mColor )
{
    pImpl_->Clear(mColor);
}

const s_uint& RenderTarget::GetWidth() const
{
    return pImpl_->GetWidth();
}

const s_uint& RenderTarget::GetHeight() const
{
    return pImpl_->GetHeight();
}

s_bool RenderTarget::SetDimensions( const s_uint& uiWidth, const s_uint& uiHeight )
{
    return pImpl_->SetDimensions(uiWidth, uiHeight);
}

const s_uint& RenderTarget::GetRealWidth() const
{
    return pImpl_->GetRealWidth();
}

const s_uint& RenderTarget::GetRealHeight() const
{
    return pImpl_->GetRealHeight();
}

void RenderTarget::SaveContentToFile( const s_str& sFileName ) const
{
    pImpl_->SaveContentToFile(sFileName);
}

s_wptr<RenderTargetImpl> RenderTarget::GetImpl() const
{
    return pImpl_;
}
