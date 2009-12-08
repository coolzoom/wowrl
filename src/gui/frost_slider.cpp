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

// TODO : Implement Slider

Slider::Slider() : Frame()
{
    lType_.PushBack("Slider");
}

Slider::~Slider()
{
}

void Slider::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);
}

void Slider::CreateGlue()
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
            pLua->Push<LuaSlider>(new LuaSlider(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}
