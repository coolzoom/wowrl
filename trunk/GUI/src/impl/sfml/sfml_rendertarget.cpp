/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           RenderTarget source          */
/*                                        */
/*                                        */

#include "frost_rendertarget.h"
#include "impl/sfml/sfml_rendertarget.h"

#include <frost_utils_color.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str RenderTargetImpl::CLASS_NAME = "GUI::RenderTargetImpl";

RenderTargetImpl::RenderTargetImpl( const s_uint& uiWidth, const s_uint& uiHeight )
{
    if (!uiWidth.IsValid() || !uiHeight.IsValid())
    {
        throw Exception(CLASS_NAME,
            "Can't create a RenderTarget with invalid dimensions : "+uiWidth+" x "+uiHeight+"."
        );
    }

    uiWidth_ = uiWidth;
    uiHeight_ = uiHeight;
    uiRealWidth_ = uiWidth/*.GetNearestPowerOfTwo()*/;
    uiRealHeight_ = uiHeight/*.GetNearestPowerOfTwo()*/;

    mRenderTarget_.Create(uiRealWidth_.Get(), uiRealHeight_.Get());
}

RenderTargetImpl::~RenderTargetImpl()
{
}

void RenderTargetImpl::Clear( const Color& mColor )
{
    mRenderTarget_.Clear(sf::Color(
        255.0f*mColor.GetR().Get(),
        255.0f*mColor.GetG().Get(),
        255.0f*mColor.GetB().Get(),
        255.0f*mColor.GetA().Get()
    ));
}

const s_uint& RenderTargetImpl::GetWidth() const
{
    return uiWidth_;
}

const s_uint& RenderTargetImpl::GetHeight() const
{
    return uiHeight_;
}

s_bool RenderTargetImpl::SetDimensions( const s_uint& uiWidth, const s_uint& uiHeight )
{
    if (!uiWidth.IsValid() || !uiHeight.IsValid())
    {
        Error(CLASS_NAME,
            "Can't create a RenderTarget with invalid dimensions : "+uiWidth+" x "+uiHeight+"."
        );
        return false;
    }

    s_uint uiNewWidth = uiWidth.GetNearestPowerOfTwo();
    s_uint uiNewHeight = uiHeight.GetNearestPowerOfTwo();
    if ((uiRealWidth_ < uiNewWidth) ||
        (uiRealHeight_ < uiNewHeight))
    {
        uiRealWidth_ = uiNewWidth;
        uiRealHeight_ = uiNewHeight;
        mRenderTarget_.Create(uiRealWidth_.Get(), uiRealHeight_.Get());
        return true;
    }
    else
    {
        uiWidth_ = uiWidth;
        uiHeight_ = uiHeight;
        return false;
    }
}

const s_uint& RenderTargetImpl::GetRealWidth() const
{
    return uiRealWidth_;
}

const s_uint& RenderTargetImpl::GetRealHeight() const
{
    return uiRealHeight_;
}

const sf::Image& RenderTargetImpl::GetImage() const
{
    return mRenderTarget_.GetImage();
}

sf::RenderImage& RenderTargetImpl::GetRenderImage()
{
    return mRenderTarget_;
}

void RenderTargetImpl::SaveContentToFile( const s_str& sFileName ) const
{

}
