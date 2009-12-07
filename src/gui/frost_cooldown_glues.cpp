/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_cooldown.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaCooldown::CLASS_NAME = "GUI::LuaCooldown";

LuaCooldown::LuaCooldown(lua_State* pLua) : LuaFrame(pLua)
{
    pCooldownParent_ = s_ptr<Cooldown>::DynamicCast(pParent_);
    if (pParent_ && !pCooldownParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
