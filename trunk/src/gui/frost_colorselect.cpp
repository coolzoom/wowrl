/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_colorselect.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ColorSelect::CLASS_NAME = "GUI::ColorSelect";

// TODO : Implement ColorSelect

ColorSelect::ColorSelect() : Frame()
{
    lType_.PushBack("ColorSelect");
}

ColorSelect::~ColorSelect()
{
}

void ColorSelect::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);
}

void ColorSelect::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(pLua->PushNew<LuaColorSelect>());
        pLua->SetGlobal(sLuaName_);
    }
}
