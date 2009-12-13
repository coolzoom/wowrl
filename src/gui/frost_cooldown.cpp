/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_cooldown.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Cooldown::CLASS_NAME = "GUI::Cooldown";

// TODO : Implement Cooldown

Cooldown::Cooldown() : Frame()
{
    lType_.PushBack("Cooldown");
}

Cooldown::~Cooldown()
{
}

void Cooldown::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);
}

void Cooldown::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(
            pLua->Push<LuaVirtualGlue>(new LuaVirtualGlue(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(
            pLua->Push<LuaCooldown>(new LuaCooldown(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}
