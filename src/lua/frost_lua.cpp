/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */


#include "lua/frost_lua.h"

#include "gui/frost_uiobject.h"
#include "gui/frost_frame.h"
#include "gui/frost_layeredregion.h"
#include "gui/frost_button.h"
#include "gui/frost_editbox.h"
#include "gui/frost_scrollingmessageframe.h"
#include "gui/frost_slider.h"
#include "gui/frost_statusbar.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_texture.h"
#include "unit/frost_unitmanager.h"
#include "unit/frost_unit.h"
#include "unit/frost_character.h"
#include "unit/frost_creature.h"
#include "scene/frost_movableobject.h"
#include "gameplay/frost_gameplay.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

void Lua::RegisterGlobalFuncs( s_ptr<Lua::State> pLua )
{
    pLua->Register("GetLocale", l_GetLocale);
    pLua->Register("GetLocalizedString", l_GetLocalizedString);
    pLua->Register("GetTime", l_GetTime);
    pLua->Register("GetTimeOfTheDay", l_GetTimeOfTheDay);
    pLua->Register("Exit", l_Exit);
}

void Lua::RegisterGUIClasses( s_ptr<Lua::State> pLua )
{
    pLua->Register<GUI::LuaUIObject>();
    pLua->Register<GUI::LuaFrame>();
    pLua->Register<GUI::LuaStatusBar>();
    pLua->Register<GUI::LuaEditBox>();
    pLua->Register<GUI::LuaScrollingMessageFrame>();
    pLua->Register<GUI::LuaButton>();
    pLua->Register<GUI::LuaSlider>();
    pLua->Register<GUI::LuaLayeredRegion>();
    pLua->Register<GUI::LuaTexture>();
    pLua->Register<GUI::LuaFontString>();
}

void Lua::RegisterUnitClass( s_ptr<Lua::State> pLua )
{
    pLua->Register<LuaUnitManager>();
    UnitManager::GetSingleton()->CreateGlue(pLua);
    pLua->Register<LuaUnit>();
    pLua->Register<LuaMovableUnit>();
    pLua->Register<LuaCharacter>();
    pLua->Register<LuaCreature>();
}

void Lua::RegisterMovableObjectClass( s_ptr<Lua::State> pLua )
{
    pLua->Register<LuaMovableObject>();
}

void Lua::RegisterGameplayClass( s_ptr<Lua::State> pLua )
{
    pLua->Register<LuaGameplay>();
}

void Lua::RegisterVectorClass( s_ptr<Lua::State> pLua )
{
    pLua->DoFile("DB/Vector.lua");
}

void Lua::RegisterKeyCodes( s_ptr<Lua::State> pLua )
{
    pLua->DoFile("DB/KeyCodes.lua");
}
