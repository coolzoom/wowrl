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
    mObjectType_ = OJBECT_TYPE_EDITBOX;
    lType_.PushBack("EditBox");
}

EditBox::~EditBox()
{
}

void EditBox::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaEditBox>(new LuaEditBox(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}
