/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_slider.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Slider::CLASS_NAME = "GUI::Slider";

// TODO : Implementer Slider

Slider::Slider() : Frame()
{
    mObjectType_ = OJBECT_TYPE_SLIDER;
    lType_.PushBack("Slider");
}

Slider::~Slider()
{
}

void Slider::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaSlider>(new LuaSlider(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}
