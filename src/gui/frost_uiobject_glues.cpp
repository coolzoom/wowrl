/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_uiobject.h"
#include "gui/frost_anchor.h"

#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaUIObject::CLASS_NAME = "GUI::LuaUIObject";

LuaUIObject::LuaUIObject( lua_State* pLua )
{
    s_str sName = lua_tostring(pLua, -1);

    lua_newtable(pLua);
    iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
    pLua_ = pLua;

    pParent_ = GUIManager::GetSingleton()->GetUIObjectByName(sName);
    if (!pParent_)
    {
        throw Exception(CLASS_NAME, "Glue missing its parent !");
    }
}

LuaUIObject::~LuaUIObject()
{
    luaL_unref(pLua_, LUA_REGISTRYINDEX, iRef_);
}

s_ptr<UIObject> LuaUIObject::GetParent()
{
    return pParent_;
}

int LuaUIObject::GetDataTable( lua_State * pLua )
{
    lua_getref(pLua, iRef_);
    return 1;
}

int LuaUIObject::_GetAlpha( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetAlpha", pLua, 1);

    mFunc.Push(pParent_->GetAlpha());

    return mFunc.Return();
}

int LuaUIObject::_GetName( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetName", pLua, 1);

    mFunc.Push(pParent_->GetName());

    return mFunc.Return();
}

int LuaUIObject::_GetObjectType( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetObjectType", pLua, 1);

    mFunc.Push(pParent_->GetObjectType());

    return mFunc.Return();
}

int LuaUIObject::_IsObjectType( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsObjectType", pLua, 1);
    mFunc.Add(0, "object type", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        mFunc.Push(pParent_->IsObjectType(mFunc.Get(0)->GetString()));
    }

    return mFunc.Return();
}

int LuaUIObject::_SetAlpha( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetAlpha", pLua);
    mFunc.Add(0, "alpha", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pParent_->SetAlpha(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaUIObject::_ClearAllPoints( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:ClearAllPoints", pLua);

    pParent_->ClearAllPoints();

    return mFunc.Return();
}

int LuaUIObject::_GetBottom( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetBottom", pLua, 1);

    mFunc.Push(pParent_->GetBottom());

    return mFunc.Return();
}

int LuaUIObject::_GetCenter( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetCenter", pLua, 2);

    Point<s_int> mP = pParent_->GetCenter();
    mFunc.Push(mP.X());
    mFunc.Push(mP.Y());

    return mFunc.Return();
}

int LuaUIObject::_GetHeight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetHeight", pLua, 1);

    mFunc.Push(pParent_->GetAppearentHeight());

    return mFunc.Return();
}

int LuaUIObject::_GetLeft( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetLeft", pLua, 1);

    mFunc.Push(pParent_->GetLeft());

    return mFunc.Return();
}

int LuaUIObject::_GetNumPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetNumPoint", pLua, 1);

    mFunc.Push(pParent_->GetNumPoint());

    return mFunc.Return();
}

int LuaUIObject::_GetParent( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetParent", pLua, 1);

    if (pParent_->GetParent() != NULL)
    {
        pParent_->GetParent()->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaUIObject::_GetPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetPoint", pLua, 5);
    mFunc.Add(0, "point ID", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        s_ptr<Anchor> pAnchor = pParent_->GetPoint(s_uint(mFunc.Get(0)->GetNumber()));
        if (pAnchor != NULL)
        {
            mFunc.Push(Anchor::GetStringPoint(pAnchor->GetPoint()));
            if (pAnchor->GetParent() != NULL)
            {
                pAnchor->GetParent()->PushOnLua(mFunc.GetState());
                mFunc.NotifyPushed();
            }
            else
                mFunc.PushNil();

            mFunc.Push(Anchor::GetStringPoint(pAnchor->GetParentPoint()));
            mFunc.Push(pAnchor->GetAbsOffsetX());
            mFunc.Push(pAnchor->GetAbsOffsetY());
        }
    }

    return mFunc.Return();
}

int LuaUIObject::_GetRight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetRight", pLua, 1);

    mFunc.Push(pParent_->GetRight());

    return mFunc.Return();
}

int LuaUIObject::_GetTop( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetTop", pLua, 1);

    mFunc.Push(pParent_->GetTop());

    return mFunc.Return();
}

int LuaUIObject::_GetWidth( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetWidth", pLua, 1);

    mFunc.Push(pParent_->GetAppearentWidth());

    return mFunc.Return();
}

int LuaUIObject::_Hide( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:Hide", pLua);

    pParent_->Hide();

    return mFunc.Return();
}

int LuaUIObject::_IsShown( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsShown", pLua, 1);

    mFunc.Push(pParent_->IsShown());

    return mFunc.Return();
}

int LuaUIObject::_IsVisible( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsVisible", pLua, 1);

    mFunc.Push(pParent_->IsVisible());

    return mFunc.Return();
}

int LuaUIObject::_RebuildCache( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:RebuildCache", pLua);

    pParent_->UpdateMaterial(true);

    return mFunc.Return();
}

int LuaUIObject::_SetAllPoints( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetAllPoints", pLua);
    mFunc.Add(0, "frame name", Lua::TYPE_STRING, true);
    mFunc.Add(0, "frame", Lua::TYPE_USERDATA, true);
    if (mFunc.Check())
    {
        s_ptr<Lua::Argument> pArg = mFunc.Get(0);
        if (pArg->IsProvided())
        {
            s_ptr<UIObject> pFrame;
            if (pArg->GetType() == Lua::TYPE_STRING)
                pFrame = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetString());
            else
                pFrame = mFunc.GetState()->Get<LuaUIObject>(pArg->GetIndex())->GetParent();
            pParent_->SetAllPoints(pFrame);
        }
        else
        {
            pParent_->SetAllPoints(NULL);
        }
    }

    return mFunc.Return();
}

int LuaUIObject::_SetHeight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetHeight", pLua);
    mFunc.Add(0, "height", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pParent_->SetAbsHeight(s_uint(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaUIObject::_SetParent( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetParent", pLua);
    mFunc.Add(0, "parent name", Lua::TYPE_STRING, true);
    mFunc.Add(0, "parent", Lua::TYPE_USERDATA, true);
    if (mFunc.Check())
    {
        s_ptr<Lua::Argument> pArg = mFunc.Get(0);
        if (pArg->IsProvided())
        {
            s_ptr<UIObject> pParent;
            if (pArg->GetType() == Lua::TYPE_STRING)
                pParent = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetString());
            else
                pParent = mFunc.GetState()->Get<LuaUIObject>(pArg->GetIndex())->GetParent();
            pParent_->SetParent(pParent);
        }
        else
        {
            pParent_->SetParent(NULL);
        }
    }

    return mFunc.Return();
}

int LuaUIObject::_SetPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetPoint", pLua);
    mFunc.Add(0, "point", Lua::TYPE_STRING);
    mFunc.Add(1, "parent name", Lua::TYPE_STRING, true);
    mFunc.Add(1, "parent", Lua::TYPE_USERDATA, true);
    mFunc.Add(2, "relative point", Lua::TYPE_STRING, true);
    mFunc.Add(3, "x offset", Lua::TYPE_NUMBER, true);
    mFunc.Add(4, "y offset", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        // point
        AnchorPoint mPoint = Anchor::GetAnchorPoint(mFunc.Get(0)->GetString());

        // parent
        s_ptr<Lua::Argument> pArg = mFunc.Get(1);
        s_ptr<UIObject> pParent;
        if (pArg->IsProvided())
        {
            if (pArg->GetType() == Lua::TYPE_STRING)
                pParent = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetString());
            else
                pParent = mFunc.GetState()->Get<LuaUIObject>(pArg->GetIndex())->GetParent();
        }

        // relativePoint
        AnchorPoint mParentPoint = ANCHOR_TOPLEFT;
        if (mFunc.IsProvided(2))
            mParentPoint = Anchor::GetAnchorPoint(mFunc.Get(2)->GetString());

        // x
        s_int iAbsX;
        if (mFunc.IsProvided(3))
            iAbsX = s_int(mFunc.Get(3)->GetNumber());

        // y
        s_int iAbsY;
        if (mFunc.IsProvided(4))
            iAbsY = s_int(mFunc.Get(4)->GetNumber());

        pParent_->SetAbsPoint(mPoint, pParent, mParentPoint, iAbsX, iAbsY);
    }

    return mFunc.Return();
}

int LuaUIObject::_SetWidth( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetWidth", pLua);
    mFunc.Add(0, "width", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pParent_->SetAbsWidth(s_uint(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaUIObject::_Show( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:Show", pLua);

    pParent_->Show();

    return mFunc.Return();
}

