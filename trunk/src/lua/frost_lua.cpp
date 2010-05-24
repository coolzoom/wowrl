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
#include "gui/frost_checkbutton.h"
#include "gui/frost_colorselect.h"
#include "gui/frost_cooldown.h"
#include "gui/frost_editbox.h"
#include "gui/frost_messageframe.h"
#include "gui/frost_modelframe.h"
#include "gui/frost_scrollframe.h"
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
#include "frost_engine_glues.h"

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
    pLua->DoFile("DB/ClassHelper.lua");
}

void Lua::RegisterEngineClass( s_ptr<Lua::State> pLua )
{
    pLua->Register<LuaEngine>();
}

void Lua::RegisterGUIClasses( s_ptr<Lua::State> pLua )
{
    pLua->Register<GUI::LuaVirtualGlue>();
    pLua->Register<GUI::LuaUIObject>();
    pLua->Register<GUI::LuaFrame>();
    pLua->Register<GUI::LuaButton>();
    pLua->Register<GUI::LuaCheckButton>();
    pLua->Register<GUI::LuaColorSelect>();
    pLua->Register<GUI::LuaCooldown>();
    pLua->Register<GUI::LuaEditBox>();
    pLua->Register<GUI::LuaMessageFrame>();
    pLua->Register<GUI::LuaModelFrame>();
    pLua->Register<GUI::LuaScrollFrame>();
    pLua->Register<GUI::LuaScrollingMessageFrame>();
    pLua->Register<GUI::LuaSlider>();
    pLua->Register<GUI::LuaStatusBar>();
    pLua->Register<GUI::LuaLayeredRegion>();
    pLua->Register<GUI::LuaTexture>();
    pLua->Register<GUI::LuaFontString>();

    RegisterKeyCodes(pLua);
    pLua->Register("SetKeyBinding", l_SetKeyBinding);
    pLua->Register("CreateFrame",   l_CreateFrame);
    pLua->Register("DeleteFrame",   l_DeleteFrame);
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
