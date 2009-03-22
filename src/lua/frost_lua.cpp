/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */


#include "frost_lua.h"

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_frame.h"
#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_gui_button.h"
#include "gui/frost_gui_editbox.h"
#include "gui/frost_gui_scrollingmessageframe.h"
#include "gui/frost_gui_slider.h"
#include "gui/frost_gui_statusbar.h"
#include "gui/frost_gui_fontstring.h"
#include "gui/frost_gui_texture.h"
#include "frost_unit.h"
#include "frost_character.h"
#include "frost_creature.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

void Lua::RegisterGlobalFuncs( s_ptr<Lua::State> pLua )
{
    pLua->Register("GetDelta", l_GetDelta);
    pLua->Register("GetLocale", l_GetLocale);
    pLua->Register("GetLocalizedString", l_GetLocalizedString);
    pLua->Register("GetTime", l_GetTime);
    pLua->Register("GetTimeOfTheDay", l_GetTimeOfTheDay);
    pLua->Register("GetMousePos", l_GetMousePos);
}

void Lua::RegisterGUIClasses( s_ptr<Lua::State> pLua )
{
    lua_State* pRawLua = pLua->GetState();
    Lunar<GUI::LuaUIObject>::Register(pRawLua);
    Lunar<GUI::LuaFrame>::Register(pRawLua);
    Lunar<GUI::LuaStatusBar>::Register(pRawLua);
    Lunar<GUI::LuaEditBox>::Register(pRawLua);
    Lunar<GUI::LuaScrollingMessageFrame>::Register(pRawLua);
    Lunar<GUI::LuaButton>::Register(pRawLua);
    Lunar<GUI::LuaSlider>::Register(pRawLua);
    Lunar<GUI::LuaLayeredRegion>::Register(pRawLua);
    Lunar<GUI::LuaTexture>::Register(pRawLua);
    Lunar<GUI::LuaFontString>::Register(pRawLua);
}

void Lua::RegisterUnitClass( s_ptr<Lua::State> pLua )
{
    lua_State* pRawLua = pLua->GetState();
    Lunar<LuaUnit>::Register(pRawLua);
    Lunar<LuaCharacter>::Register(pRawLua);
    Lunar<LuaCreature>::Register(pRawLua);
}
