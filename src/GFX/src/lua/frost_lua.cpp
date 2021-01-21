/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */


#include "lua/frost_lua.h"

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
    pLua->DoFile("ClassHelper.lua");
}

void Lua::RegisterMovableObjectClass( s_ptr<Lua::State> pLua )
{
    pLua->Register<LuaMovableObject>();
}

void Lua::RegisterGameplayClass( s_ptr<Lua::State> pLua )
{
    pLua->Register<LuaGameplay>();
}
