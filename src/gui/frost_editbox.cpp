/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_editbox.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str EditBox::CLASS_NAME = "GUI::EditBox";

// TODO : Implement EditBox

EditBox::EditBox() : Frame()
{
    lType_.PushBack("EditBox");
}

EditBox::~EditBox()
{
}

void EditBox::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);
}

void EditBox::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->NewTable();
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(
            pLua->Push<LuaEditBox>(new LuaEditBox(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}
