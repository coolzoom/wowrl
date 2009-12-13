/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_scrollframe.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ScrollFrame::CLASS_NAME = "GUI::ScrollFrame";

// TODO : Implementer ScrollFrame

ScrollFrame::ScrollFrame() : Frame()
{
    lType_.PushBack("ScrollFrame");
}

ScrollFrame::~ScrollFrame()
{
}

void ScrollFrame::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);
}

void ScrollFrame::CreateGlue()
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
            pLua->Push<LuaScrollFrame>(new LuaScrollFrame(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}
