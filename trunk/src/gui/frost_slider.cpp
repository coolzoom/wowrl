/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_slider.h"

#include "gui/frost_gui_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Slider::CLASS_NAME = "GUI::Slider";

// TODO : Implementer Slider

Slider::Slider() : Frame()
{
}

void Slider::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    LuaSlider* pNewGlue;
    pGlue_ = pNewGlue = new LuaSlider(pLua->GetState());
    pLua->Push<LuaSlider>(pNewGlue);
    pLua->SetGlobal(sName_);
}
