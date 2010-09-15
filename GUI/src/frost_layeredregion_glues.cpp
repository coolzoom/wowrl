/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_layeredregion.h"

#include "frost_uiobject.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaLayeredRegion::CLASS_NAME = "GUI::LuaLayeredRegion";

LuaLayeredRegion::LuaLayeredRegion(lua_State* pLua) : LuaUIObject(pLua)
{
    pLayeredRegionParent_ = s_ptr<LayeredRegion>::DynamicCast(pParent_);
    if (pParent_ && !pLayeredRegionParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaLayeredRegion::_SetDrawLayer(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("LayeredRegion:SetDrawLayer", pLua);
    mFunc.Add(0, "layer", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pLayeredRegionParent_->SetDrawLayer(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int LuaLayeredRegion::_GetDrawLayer(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("LayeredRegion:GetDrawLayer", pLua, 1);

    switch (pLayeredRegionParent_->GetDrawLayer())
    {
        case LAYER_BACKGROUND : mFunc.Push(s_str("BACKGROUND")); break;
        case LAYER_BORDER : mFunc.Push(s_str("BORDER")); break;
        case LAYER_ARTWORK : mFunc.Push(s_str("ARTWORK")); break;
        case LAYER_OVERLAY : mFunc.Push(s_str("OVERLAY")); break;
        case LAYER_HIGHLIGHT : mFunc.Push(s_str("HIGHLIGHT")); break;
        case LAYER_SPECIALHIGH : mFunc.Push(s_str("SPECIALHIGH")); break;
    }

    return mFunc.Return();
}
