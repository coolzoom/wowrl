#include "frost_rendertarget.h"
#include "impl/ogre/ogre_guimanager.h"
#include "impl/ogre/ogre_spritemanager.h"
#include "frost_fontmanager.h"

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
        FontManager::Delete();
    }
}
}

GUIManagerImpl::GUIManagerImpl( const s_map<s_str, s_var>& lParams )
{
    s_str  sSceneMgr      = GetValue<s_str>(lParams, "scene_manager", "s_str");
    s_uint uiScreenWidth  = GetValue<s_uint>(lParams, "width", "s_uint");
    s_uint uiScreenHeight = GetValue<s_uint>(lParams, "height", "s_uint");

    pSpriteMgr_ = s_refptr<GUI::SpriteManager>(new GUI::SpriteManager(uiScreenWidth, uiScreenHeight, sSceneMgr));
}

GUIManagerImpl::~GUIManagerImpl()
{
}

void GUIManagerImpl::Begin( s_ptr<GUI::RenderTarget> pTarget )
{
    pSpriteMgr_->Begin(pTarget);
}

void GUIManagerImpl::End()
{
    pSpriteMgr_->End();
}

void GUIManagerImpl::RenderQuad( const GUI::Quad& mQuad )
{
    pSpriteMgr_->RenderQuad(mQuad);
}

s_wptr<GUI::SpriteManager> GUIManagerImpl::GetSpriteManager()
{
    return pSpriteMgr_;
}
