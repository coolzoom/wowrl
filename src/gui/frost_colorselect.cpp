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

void ColorSelect::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaColorSelect>(new LuaColorSelect(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}
