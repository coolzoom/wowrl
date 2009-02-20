/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_anchor.h"

#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaUIObject::CLASS_NAME = "GUI::LuaUIObject";

LuaUIObject::LuaUIObject(lua_State* pLua)
{
    s_str sName = lua_tostring(pLua, 1);

    lua_newtable(pLua);
    iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
    pLua_ = pLua;

    pParent_ = GUIManager::GetSingleton()->GetUIObjectByName(sName);
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

    mFunc.Push(Lua::ReturnValue(pParent_->GetAlpha()));

    return mFunc.Return();
}

int LuaUIObject::_GetName( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetName", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetName()));

    return mFunc.Return();
}

int LuaUIObject::_GetObjectType( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetObjectType", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetObjectType()));

    return mFunc.Return();
}

int LuaUIObject::_IsObjectType( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsObjectType", pLua, 1);
    mFunc.Add(0, "type", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_bool bIsType = false;
        s_str sType = mFunc.Get(0)->GetS();
        const vector<s_str>& lType = pParent_->GetObjectTypeList();
        vector<s_str>::const_iterator iterType;
        foreach (iterType, lType)
        {
            if (sType == *iterType)
            {
                bIsType = true;
                break;
            }
        }

        mFunc.Push(Lua::ReturnValue(bIsType));
    }

    return mFunc.Return();
}

int LuaUIObject::_SetAlpha( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetAlpha", pLua);
    mFunc.Add(0, "alpha", Lua::TYPE_NUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        pParent_->SetAlpha(mFunc.Get(0)->GetF());
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

    mFunc.Push(Lua::ReturnValue(pParent_->GetBottom()));

    return mFunc.Return();
}

int LuaUIObject::_GetCenter( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetCenter", pLua, 2);

    Point<s_int> mP = pParent_->GetCenter();
    mFunc.Push(Lua::ReturnValue(mP.X()));
    mFunc.Push(Lua::ReturnValue(mP.Y()));

    return mFunc.Return();
}

int LuaUIObject::_GetHeight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetHeight", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetAbsHeight()));

    return mFunc.Return();
}

int LuaUIObject::_GetLeft( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetLeft", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetLeft()));

    return mFunc.Return();
}

int LuaUIObject::_GetNumPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetNumPoint", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetNumPoint()));

    return mFunc.Return();
}

int LuaUIObject::_GetParent( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetParent", pLua, 1);

    if (pParent_->GetParent() != NULL)
    {
        Lunar<LuaUIObject>::push(pLua, pParent_->GetParent()->GetGlue().Get());
        mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
    }

    return mFunc.Return();
}

int LuaUIObject::_GetPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetPoint", pLua, 5);
    mFunc.Add(0, "pointID", Lua::TYPE_NUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        s_ptr<Anchor> pAnchor = pParent_->GetPoint(mFunc.Get(0)->GetUI());
        if (pAnchor != NULL)
        {
            mFunc.Push(Lua::ReturnValue(Anchor::GetStringPoint(pAnchor->GetPoint())));
            if (pAnchor->GetParent() != NULL)
            {
                //lua_getglobal(pLua, pAnchor->pParent->GetName().c_str());
                Lunar<LuaUIObject>::push(pLua, pAnchor->GetParent()->GetGlue().Get());
                mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
            }
            else
                mFunc.Push(Lua::ReturnValue(Lua::RETURN_NIL));

            mFunc.Push(Lua::ReturnValue(Anchor::GetStringPoint(pAnchor->GetParentPoint())));
            mFunc.Push(Lua::ReturnValue(pAnchor->GetAbsOffsetX()));
            mFunc.Push(Lua::ReturnValue(pAnchor->GetAbsOffsetY()));
        }
    }

    return mFunc.Return();
}

int LuaUIObject::_GetRight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetRight", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetRight()));

    return mFunc.Return();
}

int LuaUIObject::_GetTop( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetTop", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetTop()));

    return mFunc.Return();
}

int LuaUIObject::_GetWidth( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetWidth", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->GetAbsWidth()));

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

    mFunc.Push(Lua::ReturnValue(pParent_->IsShown()));

    return mFunc.Return();
}

int LuaUIObject::_IsVisible( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsVisible", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pParent_->IsVisible()));

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
    mFunc.Add(0, "frameName", Lua::TYPE_STRING, VALUE_STRING, true);
    mFunc.Add(0, "frame", Lua::TYPE_USERDATA, VALUE_NONE, true);
    if (mFunc.Check())
    {
        s_ptr<Lua::Argument> pArg = mFunc.Get(0);
        if (pArg->IsProvided())
        {
            s_ptr<UIObject> pFrame;
            if (pArg->GetType() == Lua::TYPE_STRING)
                pFrame = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetS());
            else
                pFrame = Lunar<LuaUIObject>::check(pLua, pArg->GetI().Get())->GetParent();
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
    mFunc.Add(0, "height", Lua::TYPE_NUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        pParent_->SetAbsHeight(mFunc.Get(0)->GetUI());
    }

    return mFunc.Return();
}

int LuaUIObject::_SetParent( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetParent", pLua);
    mFunc.Add(0, "parentName", Lua::TYPE_STRING, VALUE_STRING, true);
    mFunc.Add(0, "parent", Lua::TYPE_USERDATA, VALUE_NONE, true);
    if (mFunc.Check())
    {
        s_ptr<Lua::Argument> pArg = mFunc.Get(0);
        if (pArg->IsProvided())
        {
            s_ptr<UIObject> pParent;
            if (pArg->GetType() == Lua::TYPE_STRING)
                pParent = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetS());
            else
                pParent = Lunar<LuaUIObject>::check(pLua, pArg->GetI().Get())->GetParent();
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
    mFunc.Add(0, "point", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(1, "parentName", Lua::TYPE_STRING, VALUE_STRING, true);
    mFunc.Add(1, "parent", Lua::TYPE_USERDATA, VALUE_NONE, true);
    mFunc.Add(2, "relativePoint", Lua::TYPE_STRING, VALUE_STRING, true);
    mFunc.Add(3, "x", Lua::TYPE_NUMBER, VALUE_INT, true);
    mFunc.Add(4, "y", Lua::TYPE_NUMBER, VALUE_INT, true);
    if (mFunc.Check())
    {
        // point
        AnchorPoint mPoint = Anchor::GetAnchorPoint(mFunc.Get(0)->GetS());

        // parent
        s_ptr<Lua::Argument> pArg = mFunc.Get(1);
        s_ptr<UIObject> pParent;
        if (pArg->IsProvided())
        {
            if (pArg->GetType() == Lua::TYPE_STRING)
            {
                pParent = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetS());
            }
            else
            {
                pParent = Lunar<LuaUIObject>::check(pLua, pArg->GetI().Get())->GetParent();
            }
        }

        // relativePoint
        AnchorPoint mParentPoint = ANCHOR_TOPLEFT;
        if (mFunc.IsProvided(2))
            mParentPoint = Anchor::GetAnchorPoint(mFunc.Get(2)->GetS());

        // x
        s_int iAbsX;
        if (mFunc.IsProvided(3))
            iAbsX = mFunc.Get(3)->GetI();

        // y
        s_int iAbsY;
        if (mFunc.IsProvided(4))
            iAbsY = mFunc.Get(4)->GetI();

        pParent_->SetAbsPoint(mPoint, pParent, mParentPoint, iAbsX, iAbsY);
    }

    return mFunc.Return();
}

int LuaUIObject::_SetWidth( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetWidth", pLua);
    mFunc.Add(0, "width", Lua::TYPE_NUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        pParent_->SetAbsWidth(mFunc.Get(0)->GetUI());
    }

    return mFunc.Return();
}

int LuaUIObject::_Show( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:Show", pLua);

    pParent_->Show();

    return mFunc.Return();
}

