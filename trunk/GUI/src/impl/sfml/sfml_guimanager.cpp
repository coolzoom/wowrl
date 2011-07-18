#include "frost_rendertarget.h"
#include "impl/sfml/sfml_rendertarget.h"
#include "impl/sfml/sfml_guimanager.h"
#include "frost_fontmanager.h"
#include "impl/sfml/sfml_texturemanager.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace Frost;

const s_str GUIManagerImpl::CLASS_NAME = "GUIManagerImpl";

template<class T>
T GetValue( const s_map<s_str, s_var>& lParams, const s_str& sName, const s_str& sType )
{
    s_map<s_str, s_var>::const_iterator iter = lParams.Get(sName);

    if (iter == lParams.End())
        throw Exception("GUIManagerImpl", "Missing \""+sName+"\" parameter in Initialize().");
    if (!iter->second.IsOfType<T>())
        throw Exception("GUIManagerImpl", "\""+sName+"\" must be a "+sType+" (be sure to explicitely set the type).");

    return iter->second.Get<T>();
}

namespace Frost
{
namespace GUI
{
    void Initialize( const s_map<s_str, s_var>& lParams )
    {
    }

    void Close()
    {
        GUI::FontManager::Delete();
        GUI::TextureManager::Delete();
    }
}
}

GUIManagerImpl::GUIManagerImpl( const s_map<s_str, s_var>& lParams )
{
    pWindow_ = GetValue< s_ptr<sf::RenderWindow> >(lParams, "window", "s_ptr<sf::RenderWindow>");
}

GUIManagerImpl::~GUIManagerImpl()
{
}

void GUIManagerImpl::Begin( s_ptr<GUI::RenderTarget> pTarget )
{
    if (pTarget)
    {
        pRenderImage_ = &pTarget->GetImpl()->GetRenderImage();
        pRenderTarget_ = pRenderImage_;
    }
    else
    {
        pRenderTarget_ = pWindow_;
        pRenderImage_ = nullptr;
    }
}

void GUIManagerImpl::End()
{
    if (pRenderImage_)
        pRenderImage_->Display();

    pRenderTarget_ = nullptr;
    pRenderImage_ = nullptr;
}

s_ptr<sf::RenderWindow> GUIManagerImpl::GetRenderWindow()
{
    return pWindow_;
}

s_ptr<const sf::RenderTarget> GUIManagerImpl::GetCurrentRenderTarget() const
{
    if (!pRenderTarget_)
        return pWindow_;
    else
        return pRenderTarget_;
}

s_ptr<sf::RenderTarget> GUIManagerImpl::GetCurrentRenderTarget()
{
    if (!pRenderTarget_)
        return pWindow_;
    else
        return pRenderTarget_;
}
