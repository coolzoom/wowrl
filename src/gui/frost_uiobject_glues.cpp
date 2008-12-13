/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_uiobject.h"

#include "frost_gui.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

int UIObject::_GetAlpha( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetAlpha", pLua, 1);

    mFunc.Push(Lua::ReturnValue(fAlpha_));

    return mFunc.Return();
}

int UIObject::_GetName( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetName", pLua, 1);

    mFunc.Push(Lua::ReturnValue(sName_));

    return mFunc.Return();
}

int UIObject::_GetObjectType( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetObjectType", pLua, 1);

    mFunc.Push(Lua::ReturnValue(lType_.back()));

    return mFunc.Return();
}

int UIObject::_IsObjectType( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsObjectType", pLua, 1);
    mFunc.Add(0, "type", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_bool bIsType = false;
        s_str sType = mFunc.Get(0)->GetS();
        vector<s_str>::iterator iterType;
        foreach (iterType, lType_)
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

int UIObject::_SetAlpha( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetAlpha", pLua);
    mFunc.Add(0, "alpha", LUA_TNUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        fAlpha_ = mFunc.Get(0)->GetF();
    }

    return mFunc.Return();
}

int UIObject::_ClearAllPoints( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:ClearAllPoints", pLua);

    ClearAllPoints();

    return mFunc.Return();
}

int UIObject::_GetBottom( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetBottom", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetBottom()));

    return mFunc.Return();
}

int UIObject::_GetCenter( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetCenter", pLua, 2);

    Point<s_int> mP = GetCenter();
    mFunc.Push(Lua::ReturnValue(mP.X()));
    mFunc.Push(Lua::ReturnValue(mP.Y()));

    return mFunc.Return();
}

int UIObject::_GetHeight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetHeight", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetAbsHeight()));

    return mFunc.Return();
}

int UIObject::_GetLeft( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetLeft", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetLeft()));

    return mFunc.Return();
}

int UIObject::_GetNumPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetNumPoint", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetNumPoint()));

    return mFunc.Return();
}

int UIObject::_GetParent( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetParent", pLua, 1);

    if (pParent_ != NULL)
    {
        //lua_getglobal(pLua, pParent_->GetName().c_str());
        Lunar<UIObject>::push(pLua, pParent_.Get());
        mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
    }

    return mFunc.Return();
}

int UIObject::_GetPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetPoint", pLua, 5);
    mFunc.Add(0, "pointID", LUA_TNUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        s_ptr<Anchor> pAnchor = GetPoint(mFunc.Get(0)->GetUI());
        if (pAnchor != NULL)
        {
            mFunc.Push(Lua::ReturnValue(Anchor::GetStringPoint(pAnchor->GetPoint())));
            if (pAnchor->GetParent() != NULL)
            {
                //lua_getglobal(pLua, pAnchor->pParent->GetName().c_str());
                Lunar<UIObject>::push(pLua, pAnchor->GetParent().Get());
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

int UIObject::_GetRight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetRight", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetRight()));

    return mFunc.Return();
}

int UIObject::_GetTop( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetTop", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetTop()));

    return mFunc.Return();
}

int UIObject::_GetWidth( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:GetWidth", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetAbsWidth()));

    return mFunc.Return();
}

int UIObject::_Hide( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:Hide", pLua);

    Hide();

    return mFunc.Return();
}

int UIObject::_IsShown( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsShown", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsShown()));

    return mFunc.Return();
}

int UIObject::_IsVisible( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:IsVisible", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsVisible()));

    return mFunc.Return();
}

int UIObject::_RebuildCache( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:RebuildCache", pLua);

    UpdateMaterial(true);

    return mFunc.Return();
}

int UIObject::_SetAllPoints( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetAllPoints", pLua);
    mFunc.Add(0, "frameName", LUA_TSTRING, VALUE_STRING, true);
    mFunc.Add(0, "frame", LUA_TUSERDATA, VALUE_NONE, true);
    if (mFunc.Check())
    {
        s_ptr<Lua::Argument> pArg = mFunc.Get(0);
        if (pArg->IsProvided())
        {
            s_ptr<UIObject> pFrame;
            if (pArg->GetType() == LUA_TSTRING)
                pFrame = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetS());
            else
                pFrame = Lunar<UIObject>::check(pLua, pArg->GetI().Get());
            SetAllPoints(pFrame);
        }
        else
        {
            SetAllPoints(NULL);
        }
    }

    return mFunc.Return();
}

int UIObject::_SetHeight( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetHeight", pLua);
    mFunc.Add(0, "height", LUA_TNUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        if (lAnchorStack_.size() <= 1)
            uiAbsHeight_ = mFunc.Get(0)->GetUI();
    }

    return mFunc.Return();
}

int UIObject::_SetParent( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetParent", pLua);
    mFunc.Add(0, "parentName", LUA_TSTRING, VALUE_STRING, true);
    mFunc.Add(0, "parent", LUA_TUSERDATA, VALUE_NONE, true);
    if (mFunc.Check())
    {
        s_ptr<Lua::Argument> pArg = mFunc.Get(0);
        if (pArg->IsProvided())
        {
            s_ptr<UIObject> pParent;
            if (pArg->GetType() == LUA_TSTRING)
                pParent = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetS());
            else
                pParent = Lunar<UIObject>::check(pLua, pArg->GetI().Get());
            SetParent(pParent);
        }
        else
        {
            SetParent(NULL);
        }
    }

    return mFunc.Return();
}

int UIObject::_SetPoint( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetPoint", pLua);
    mFunc.Add(0, "point", LUA_TSTRING, VALUE_STRING);
    mFunc.Add(1, "parentName", LUA_TSTRING, VALUE_STRING, true);
    mFunc.Add(1, "parent", LUA_TUSERDATA, VALUE_NONE, true);
    mFunc.Add(2, "relativePoint", LUA_TSTRING, VALUE_STRING, true);
    mFunc.Add(3, "x", LUA_TNUMBER, VALUE_INT, true);
    mFunc.Add(4, "y", LUA_TNUMBER, VALUE_INT, true);
    if (mFunc.Check())
    {
        // point
        AnchorPoint mPoint = Anchor::GetAnchorPoint(mFunc.Get(0)->GetS());

        // parent
        s_ptr<Lua::Argument> pArg = mFunc.Get(1);
        s_ptr<UIObject> pParent;
        if (pArg->IsProvided())
        {
            if (pArg->GetType() == LUA_TSTRING)
            {
                pParent = GUIManager::GetSingleton()->GetUIObjectByName(pArg->GetS());
            }
            else
            {
                pParent = Lunar<UIObject>::check(pLua, pArg->GetI().Get());
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

        SetPoint(mPoint, pParent, mParentPoint, iAbsX, iAbsY);
    }

    return mFunc.Return();
}

int UIObject::_SetWidth( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:SetWidth", pLua);
    mFunc.Add(0, "width", LUA_TNUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        if (lAnchorStack_.size() <= 1)
            uiAbsWidth_ = mFunc.Get(0)->GetUI();
    }

    return mFunc.Return();
}

int UIObject::_Show( lua_State* pLua )
{
    Lua::Function mFunc("UIObject:Show", pLua);

    Show();

    return mFunc.Return();
}

